/*
 * Core-independent driver for WS2812B addressable RGB LEDs (aka NeoPixels).
 * No bit-banging, the communication protocol is handled by hardware peripherals
 * saving lots of CPU cycles.
 *
 * Connect the LED(s) DIN pin to pin D4 (PC6) of the Arduino Nano Every using the
 * RC network below. With this circuit the MCU can run at normal speed (16 MHz).
 * Without this circuit you must slow down the MCU to about 12 MHz (25%).
 *
 *        D1
 *       1N4148
 * D4 >---|>|--+----+-----> to pin Din of WS2812B
 *             |    |
 *        C1  ---   -  R1
 *     470 pF ---  | | 390 ohm
 *             |    -
 *             |    |
 * GND >-------+----+-----> GND
 *
 * Don't forget to connect its +5V power supply.
 *
 * If the CPU is slowed down, baud rates and delay times must be adjusted accordingly.
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

/*
 * Define (uncomment) MCU_SLOW_DOWN if you want to slow down the MCU.
 */
//#define MCU_SLOW_DOWN

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
