#include "cJSON.h"
#include "telemetry.h"
#include "esp_err.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "portmacro.h"
#include "wifi.h"

#define HTTP_HOST CONFIG_HTTP_HOST
#define HTTP_PATH CONFIG_HTTP_PATH

#define MAXIMUM_RETRY_ATTEMPTS 3
#define QUEUE_DEPTH            8
#define TICKS_TO_WAIT          0

static const char *TAG = "telemetry";

static QueueHandle_t s_queue = NULL;
static TaskHandle_t s_task_handle = NULL;

static void telemetry_backoff(int attempt) {
    uint32_t cap = 30000;
    uint32_t base = 1000UL << (attempt > 5 ? 5 : attempt);
    uint32_t wait = esp_random() % (base < cap ? base : cap);

    vTaskDelay(pdMS_TO_TICKS(wait));
}

static esp_err_t telemetry_send_batch(const telemetry_reading_t *batch) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "timestamp", 1787253656);
    cJSON_AddNumberToObject(root, "uptime", 3401);
    cJSON_AddStringToObject(root, "sensor_id", "ad334ccf-c1d5-44a0-a908-e0bbdf5ae7d2");

    char *json = cJSON_PrintUnformatted(root);

    // Prepare and send to the API
    esp_http_client_config_t config = {
		.url = "http://dev.basileaelterman.com/",
		.method = HTTP_METHOD_POST,
	};
	
	esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, json, strlen(json));

    esp_err_t response_status = esp_http_client_perform(client);

    if (response_status == ESP_OK) {
        ESP_LOGI(TAG, "Successfully sent data to host");
    } else {
        ESP_LOGE(TAG, "Failed to send data to host");
    }

    cJSON_free(json);
    cJSON_Delete(root);

    esp_http_client_cleanup(client);
    
    return response_status;
}

static void telemetry_task(void *arg) {
	while (1) {
        wifi_wait_for_connection(portMAX_DELAY);
		
        // Wait for a telemetry reading to be available in the queue
		telemetry_reading_t reading;
		xQueueReceive(s_queue, &reading, portMAX_DELAY);

        // Attempt to send the telemetry readings to the server
        int attempts = 0;
        
        while (attempts < MAXIMUM_RETRY_ATTEMPTS) {
            if (telemetry_send_batch(&reading) == ESP_OK) {
                break;
            }

            telemetry_backoff(attempts);
            attempts++;
        }
    }
}

esp_err_t telemetry_init(void) {
    s_queue = xQueueCreate(QUEUE_DEPTH, sizeof(telemetry_reading_t));
	
	if (s_queue == NULL) {
		return ESP_ERR_NO_MEM;
	}
	
	// Create and run the telemetry task
	BaseType_t xReturned = xTaskCreate(telemetry_task, 
                                       "telemetry_task", 
                                       8192, 
                                       NULL, 
                                       5, 
                                       &s_task_handle);
	
	if (xReturned == pdPASS) {
		return ESP_OK;
	}
	
	return ESP_FAIL;
}

esp_err_t telemetry_enqueue(const telemetry_reading_t *reading) {
	if (reading == NULL) {
		return ESP_ERR_INVALID_ARG;
	}
	if (s_queue == NULL) {
		return ESP_ERR_INVALID_STATE;
	}

    // Attempt to send the reading to the queue
    BaseType_t is_queued = xQueueSend(s_queue, reading, TICKS_TO_WAIT);

	if (is_queued != pdTRUE) {
		return ESP_FAIL;
	}
    
	return ESP_OK;
}