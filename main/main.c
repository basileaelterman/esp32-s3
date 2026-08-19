#include "esp_err.h"
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "telemetry.h"
#include "wifi.h"
#include "nvs.h"

static const char *TAG = "app";

void app_main(void)
{
	// Flash Non-Volatile Storage to the board
	esp_err_t nvs_status = nvs_flash_init();
	
	if (nvs_status == ESP_ERR_NVS_NO_FREE_PAGES || nvs_status == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		nvs_status = nvs_flash_init();
	}
	
	ESP_ERROR_CHECK(nvs_status);
	
	// Establish Wi-Fi connection
	esp_err_t wifi_status = wifi_init_sta();
	
	if (wifi_status == ESP_OK) {
		ESP_LOGI(TAG, "Boot Wi-Fi connected! Starting in ONLINE mode...");
	} else {
		ESP_LOGW(TAG, "Boot Wi-Fi connection failed. Starting in OFFLINE mode...");
	}
	
	// Run telemetry tasks
	ESP_ERROR_CHECK(telemetry_init());
}