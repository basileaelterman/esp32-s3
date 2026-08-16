#ifndef WIFI_H
#define WIFI_H

#include "esp_err.h"

// Connect to an access point.
esp_err_t wifi_init_sta(void);

#endif