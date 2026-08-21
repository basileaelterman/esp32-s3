#ifndef SNTP_H
#define SNTP_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the time.
 * 
 * @return esp_err_t
 */
esp_err_t sntp_init(void);

#ifdef __cplusplus
}
#endif

#endif // SNTP_H