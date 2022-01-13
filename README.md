# mbed-os-example-get-battery-level

This repository is tested on the following evaluation boards:
* B-L072Z-LRWAN1
* NUCLEO-F303K8
* NUCLEO-F042K6

Most of other evaluation boards which have an stm32 MCU should be able to execute `main.cpp` without any modifications.

The variable `vdda` is the power supply voltage (VDDA) for an A/D converter integrated in an STM32 MCU,
which equals to the power supply voltage (VDD) for the MCU itself. You can treat `vdda` as VDD.


```c++
#include "mbed.h"

int main()
{
    AnalogIn adc_vref(PinName::ADC_VREF);

    // Read VREFINT_CAL (always takes a constant value)
    uint16_t vrefint_cal = *((uint16_t *)VREFINT_CAL_ADDR);
    // Calc VDDA_CHARAC (VREFINT_CAL_VREF is the mV version of VDDA_CHARAC)
    float vdda_charac = VREFINT_CAL_VREF / 1000.0f;
    while (true) {
        // Read VREFINT (the 12-bit output is left-aligned)
        uint16_t vrefint = adc_vref.read_u16() >> 4;
        // Calc VDDA (= VDD)
        float vdda = vdda_charac * vrefint_cal / vrefint;
        printf("VDDA: %f [v]\n", vdda);
        // Sleep for 10 [s].
        ThisThread::sleep_for(10s);
    }
    return 0;
}
```

Here's the result:
```
VDDA: 3.322709 [v]
VDDA: 3.320504 [v]
VDDA: 3.320504 [v]
VDDA: 3.322709 [v]
VDDA: 3.320504 [v]
VDDA: 3.320504 [v]
VDDA: 3.320504 [v]
```

We ran the program on B-L072Z-LRWAN1 with 1.5V * 3 batteries (4.5V in total). VDDA stayed around 3.3V until the battery power supply went down below 3.3V, then kept going down until power off. 


See [Calculating the actual VDDA voltage using the internal reference voltage (p331)](https://www.st.com/resource/en/reference_manual/dm00108281-ultralowpower-stm32l0x2-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
for details and technical backgrounds.
