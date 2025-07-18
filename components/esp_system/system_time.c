/*
 * SPDX-FileCopyrightText: 2020-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_system.h"
#include "esp_attr.h"

#include "soc/rtc.h"

#include "freertos/FreeRTOS.h"

#include "sdkconfig.h"

#include "esp_rtc_time.h"

#include "esp_private/startup_internal.h"

// A component in the build should provide strong implementations that make use of
// and actual hardware timer to provide timekeeping functions.
int64_t __attribute__((weak)) esp_system_get_time(void)
{
    int64_t t = 0;
    t = (esp_rtc_get_time_us() - g_startup_time);
    return t;
}

uint32_t __attribute__((weak)) esp_system_get_time_resolution(void)
{
    return 1000000000L / rtc_clk_slow_freq_get_hz();
}
