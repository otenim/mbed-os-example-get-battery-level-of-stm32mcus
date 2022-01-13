# mbed-os-example-get-battery-level

This repository is tested on the following evaluation boards:
* B-L072Z-LRWAN1
* NUCLEO-WL55JC1
* NUCLEO-WL55JC2

Most of other evaluation boards which implement stm32 MCUs should be able to execute `main.cpp` without any modifications.

```c++
#include "mbed.h"

int main()
{
    AnalogIn adc_vref(PinName::ADC_VREF);

    // Read VREFINT_CAL (always takes a constant value)
    uint16_t vrefint_cal = *((uint16_t *)VREFINT_CAL_ADDR);
    // Calc VREFINT_CHARAC (VREFINT_CAL_VREF is the mV version of VREFINT_CHARAC)
    float vrefint_charac = VREFINT_CAL_VREF / 1000.0f;
    while (true) {
        // Read VREFINT (the 12-bit output is left-aligned)
        uint16_t vrefint = adc_vref.read_u16() >> 4;
        // Calc VDD
        float vdd = vrefint_charac * vrefint_cal / vrefint;
        printf("VDD: %f [v]\n", vdd);
        // Sleep for 10 [s].
        ThisThread::sleep_for(10s);
    }
    return 0;
}
```
