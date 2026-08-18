#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A single sensor reading handed to the telemetry subsystem.
 */
typedef struct {
    int64_t timestamp;
    uint32_t uptime;
    uint32_t sensor_id;
} telemetry_reading_t;

/**
 * Initialize telemetry queue.
 * 
 * @return esp_err_t
 */
esp_err_t telemetry_init(void);

/**
 * Add telemetry data to the queue.
 * 
 * @param reading The telemetry reading to add.
 * @return esp_err_t
 */
esp_err_t telemetry_enqueue(const telemetry_reading_t *reading);

#ifdef __cplusplus
}
#endif

#endif // TELEMETRY_H