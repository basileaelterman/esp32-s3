#include "esp_err.h"
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include "wifi.h"
#include "nvs.h"

static const char *TAG = "app";

void app_main(void)
{
	esp_err_t ret = nvs_flash_init();
	
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	
	ESP_ERROR_CHECK(ret);
	
	if (CONFIG_LOG_MAXIMUM_LEVEL > CONFIG_LOG_DEFAULT_LEVEL) {
		esp_log_level_set("wifi", CONFIG_LOG_MAXIMUM_LEVEL);
	}
	
	ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
	wifi_init_sta();
}