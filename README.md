# CI_WS2812B
============

ATmega4809 Core-independent driver for WS2812B addressable RGB LEDs (aka NeoPixels).
No bit-banging, the communication protocol is handled by hardware peripherals, saving lots of CPU cycles.

Connect the LED(s) DIN pin to pin D4 (PC6) of the Arduino Nano Every.
Don't forget to connect its power supply.

**IMPORTANT NOTE**
*This library makes the CPU run at about 12 MHz, adjust baud rates and delay times accordingly!

The reason for this is that the Arduino Nano Every runs at 16 MHz, not at 20 MHz as advertised.
Now timer prescaler values that would work at 20 MHz come out slightly wrong at 16 MHz. Slowing down the CPU fixes this.

Peripherals used: SPI, TCB2 (TIMERB2), and LUT1 and LUT2 of the CCL.

Board: Arduino Nano Every (ATmega4809)

Developped on Arduino IDE 1.8.15
