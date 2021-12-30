/*
 * Core-independent driver for WS2812B addressable RGB LEDs (aka NeoPixels).
 * No bit-banging, the communication protocol is handled by hardware peripherals
 * saving lots of CPU cycles.
 *
 * Connect the LED(s) DIN pin to pin D4 (PC6) of the Arduino Nano Every.
 * Don't forget to connect its power supply.
 *
 * IMPORTANT NOTE:
 * This library makes the CPU run at about 12 MHz, adjust baud rates and delay times accordingly!
 * The reason for this is that the Arduino Nano Every runs at 16 MHz, not at 20 MHz as advertised.
 * Now timer prescaler values that work at 20 MHz come out just wrong at 16 MHz :-( Slowing down
 * the CPU fixes this.
 *
 * This driver uses SPI, TCB2 (TIMERB2), and LUT1 and LUT2 of the CCL.
 *
 * Author: Clemens at Elektor
 * Date: 30/12/2021
 *
 * Board: Arduino Nano Every (ATmega4809)
 * Developped on Arduino IDE 1.8.15
 */

#ifndef __CI_WS2812B_H__
#define __CI_WS2812B_H__

/*
 * TODO
 * - Slow down CPU only during SPI transfers?
 * - Make SPI interrupt driven?
 */

typedef struct
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
}
ci_ws2812b_rgb_t;


// Core-independent (CI) driver for WS2812B addressable RGB LEDs (aka NeoPixels).
class CI_WS2812B
{
public:
  void begin(void);
  void write(uint8_t red, uint8_t green, uint8_t blue);
  void write_string(int number_of_leds, ci_ws2812b_rgb_t led_data[]);
  
private:
  void TCB2_init(void);
  void SPI_init(void);
  void SPI_transfer(uint8_t data);
  void CCL_init(void);
};

#endif /* __CI_WS2812B_H__ */
