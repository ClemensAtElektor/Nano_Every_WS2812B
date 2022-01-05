# CI_WS2812B

Arduino Nano Every (ATmega4809) core-independent driver for WS2812B addressable RGB LEDs (aka NeoPixels).
No bit-banging, the communication protocol is handled by hardware peripherals, saving lots of CPU cycles.

**Installation**

This library can be installed from the Arduino IDE's library manager, just search on Nano Every WS2812B.
You can also copy this folder with all its files into the 'libraries' folder of the 'Sketchbook' folder:

[path]/[to]/Sketchbook/libraries/Nano_Every_WS2812B

**Usage**

Connect the LED(s) DIN pin to pin D4 (PC6) of the Arduino Nano Every.
Don't forget to connect its power supply too.

**About**

This library is based on Microchip's application note [Core Independent Nightlight Using Configurable Custom
Logic on ATtiny1617](https://ww1.microchip.com/downloads/en/Appnotes/00002387B.pdf) which explains how it works. However, the ATtiny1617 (@ 20 MHz) is a bit different from the ATmega4809 and the Nano Every pinning & clock frequency (16 MHz) plus the timer constraints imposed by the Arduino API required adapting the code in sometimes rather subtle ways. An oscilloscope is practical for debugging the output signal in case you have problems.

Peripherals used: SPI, TCB2 (TIMERB2), and LUT1 and LUT2 of the CCL.

Board: Arduino Nano Every (ATmega4809)

Developped on Arduino IDE 1.8.15

**IMPORTANT NOTE**

*This library makes the CPU run at about 12 MHz, adjust baud rates and delay times accordingly!*

The reason for this is that the Arduino Nano Every runs at 16 MHz, not at 20 MHz as advertised.
Now timer prescaler values that would work at 20 MHz come out slightly wrong at 16 MHz. Slowing down the CPU fixes this.
