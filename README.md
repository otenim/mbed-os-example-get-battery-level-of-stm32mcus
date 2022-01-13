# mbed-os-example-get-battery-level

This repository is tested on the following evaluation boards:
* B-L072Z-LRWAN1
* NUCLEO-F303K8
* NUCLEO-F042K6

Most of other evaluation boards which have an stm32 MCU should be able to execute `main.cpp` without any modifications.

The variable `vdda` is the power supply voltage for an A/D converter integrated in the STM32 MCU,
which equals to the power supply voltage (VDD) for the MCU itself. You can treat `vdda` as VDD.

`vdda` will keep a nearly constant value if usb-powered.  
`vdda` will gradually go down as time passes if battery-powered.

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

Here's the output:
```
VDD: 3.322709 [v]
VDD: 3.320504 [v]
VDD: 3.320504 [v]
VDD: 3.322709 [v]
VDD: 3.320504 [v]
VDD: 3.320504 [v]
VDD: 3.320504 [v]
```

See [Calculating the actual VDDA voltage using the internal reference voltage (p331)](https://www.st.com/resource/en/reference_manual/dm00108281-ultralowpower-stm32l0x2-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
for details and technical backgrounds.
