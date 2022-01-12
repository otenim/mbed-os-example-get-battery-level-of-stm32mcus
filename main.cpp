/* mbed Microcontroller Library
 * Copyright (c) 2022 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

int main()
{
    AnalogIn adc_vref(PinName::ADC_VREF);

    // Read VREFINT_CAL (always takes a constant value)
    uint16_t vrefint_cal = *((uint16_t *)VREFINT_CAL_ADDR);
    while (true) {
        // Read VREFINT (the 12-bit output is left-aligned)
        uint16_t vrefint = adc_vref.read_u16() >> 4;
        // Calc VDDA
        float vdda = (VREFINT_CAL_VREF / 1000.0f) * vrefint_cal / vrefint;
        printf("VDDA: %f [v]\n", vdda);
        // Sleep for 10 [s].
        ThisThread::sleep_for(10s);
    }
    return 0;
}