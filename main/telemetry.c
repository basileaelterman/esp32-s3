#include "telemetry.h"
#include "esp_err.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "portmacro.h"
#include "wifi.h"

#define MAXIMUM_RETRY_ATTEMPTS 3
#define QUEUE_DEPTH            8
#define TICKS_TO_WAIT          0

QueueHandle_t s_queue = NULL;
TaskHandle_t xHandle = NULL;

static void telemetry_backoff(int attempt) {

}

static esp_err_t telemetry_send_batch(const telemetry_reading_t *batch) {

}

static void telemetry_task(void *arg) {
	while (1) {
        // Sleep until there's a Wi-Fi connection
        wifi_wait_for_connection(portMAX_DELAY);
		
        // Wait for a telemetry reading to be available in the queue
		telemetry_reading_t reading;
		BaseType_t is_received = xQueueReceive(s_queue, &reading, portMAX_DELAY);

		if (is_received != pdTRUE) {
            continue;
		}

        // Attempt to send the telemetry readings to the server
        int s_attempts = 0;
        
        while (s_attempts < MAXIMUM_RETRY_ATTEMPTS) {
            if (telemetry_send_batch(&reading) == ESP_OK) {
                break;
            }

            telemetry_backoff(s_attempts);
            s_attempts++;
        }

        // All attempts failed, log the error.
        // ...
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
                                       4096, 
                                       NULL, 
                                       5, 
                                       &xHandle);
	
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