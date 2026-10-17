/**
 ******************************************************************************
 * @file    app_nrf.h
 * @brief   nRF24L01+ radio layer, profile selector, and TX task.
 *
 * @author  Mohammadali Abbasi <m.elec76a@gmail.com>
 * @date    2026
 * @license Apache-2.0
 ******************************************************************************
 */
#ifndef APP_NRF_H
#define APP_NRF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief  Select one of ten pre-shared RF profiles and apply it to the radio.
 * @param  mode  Valid range 1..10; out-of-range values fall back to 1.
 */
void apply_mode(uint8_t mode);

#ifdef __cplusplus
}
#endif

#endif /* APP_NRF_H */
