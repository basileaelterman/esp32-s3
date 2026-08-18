#ifndef WIFI_H
#define WIFI_H

#include "esp_err.h"
#include "portmacro.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize Wi-Fi in station mode.
 * 
 * @return esp_err_t
 */
esp_err_t wifi_init_sta(void);

/**
 * Wait for Wi-Fi connection.
 * 
 * @return void
 */
void wifi_wait_for_connection(TickType_t ticks_to_wait);

#ifdef __cplusplus
}
#endif

#endif // WIFI_H