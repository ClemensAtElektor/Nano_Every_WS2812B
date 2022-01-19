# CI_WS2812B

Arduino Nano Every (ATmega4809) core-independent driver for WS2812B addressable RGB LEDs (aka NeoPixels).
No bit-banging, the communication protocol is handled by hardware peripherals, saving lots of CPU cycles.

**Installation**

This library can be installed from the Arduino IDE's library manager, just search on Nano Every WS2812B.
You can also copy this folder with all its files into the 'libraries' folder of the 'Sketchbook' folder:

[path]/[to]/Sketchbook/libraries/Nano_Every_WS2812B

**Usage**

![RC delay circuit](https://github.com/ClemensAtElektor/Nano_Every_WS2812B/blob/main/extras/ATmega4809-WS2812B-driver-16MHz-RC-network.png)

Connect pin D4 (PC6) of the Arduino Nano Every to the anode (left) of diode D1. Connect DIN pin of the WS2812B LED (string) to the cathode (right) of D1.
Don't forget to connect the +5V power supply too.

With this circuit the Arduino Nano Every can run at its normal clock speed of 16 MHz (and not 20 MHz as advertised by Arduino). Without this circuit the MCU must be slowed down about 25%. This can be achieved by defining MCU_SLOW_DOWN at the top of the file Nano_Every_WS2812B.h.

**About**

This library is based on Microchip's application note [Core Independent Nightlight Using Configurable Custom
Logic on ATtiny1617](https://ww1.microchip.com/downloads/en/Appnotes/00002387B.pdf) which explains how it works. However, the ATtiny1617 (@ 20 MHz) is a bit different from the ATmega4809 and the Nano Every pinning & clock frequency (16 MHz) plus the timer constraints imposed by the Arduino API required adapting the code in sometimes rather subtle ways. An oscilloscope is practical for debugging the output signal in case you have problems.

Peripherals used: SPI, TCB2 (TIMERB2), and LUT1 and LUT2 of the CCL.

Board: Arduino Nano Every (ATmega4809)

Developped on Arduino IDE 1.8.15

**NOTE**

*If the CPU is slowed down, adjust baud rates and delay times accordingly!*
