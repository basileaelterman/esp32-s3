#include "esp_err.h"
#include "esp_log.h"
#include "esp_netif_sntp.h"

#define SNTP_SERVER   "pool.ntp.org"
#define MAXIMUM_DELAY pdMS_TO_TICKS(10000)

static const char *TAG = "sntp";

esp_err_t sntp_init(void)
{
	esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG(SNTP_SERVER);
	esp_netif_sntp_init(&config);
	
	if (esp_netif_sntp_sync_wait(MAXIMUM_DELAY) != ESP_OK) {
		ESP_LOGW(TAG, "Failed to initialize system time");
		return ESP_FAIL;
	}
	
	ESP_LOGI(TAG, "Successfully initialized system time");
	return ESP_OK;
}