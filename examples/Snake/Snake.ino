/*
 * Connect LED (string) to pin D4 (PC6) of the Arduino Nano Every.
 * 
 * Author: Clemens at Elektor
 * Date: 30/12/2021
 *
 * Board: Arduino Nano Every
 * Arduino IDE 1.8.15
 */
#include <Nano_Every_WS2812B.h>
CI_WS2812B LEDs;

const uint8_t number_of_leds = 16; // The number of connected RGB LEDs. 

void draw(uint8_t color, uint8_t intensity, uint16_t snake, uint8_t number_of_leds)
{
  uint8_t red, green, blue;
  
  for (uint16_t i=0x0001; i!=0; i<<=1)
  {
    red = 0;
    green = 0;
    blue = 0;
    if ((i&snake)!=0)
    {
      if (color&0x01) red = intensity;
      if (color&0x02) green = intensity;
      if (color&0x04) blue = intensity;
    }
    LEDs.write(red,green,blue);
  }
  delayMicroseconds(50); // Latch the data.
}

void setup(void)
{
  LEDs.begin();
  // From now on the CPU runs about 25% slower.
  Serial.begin(144000); // About 25% higher to obtain 115200 baud.
  Serial.println("Core-independent WS2812B driver");
  Serial.println("This driver makes the CPU run at about 12 MHz.");
  Serial.println("Adjust baud rates and delay times etc. accordingly.");
  Serial.println("running...");
}

void loop(void)
{
  static uint8_t color = 1;
  static uint8_t intensity = 60;
  static uint32_t snake = 0x3f;

  draw(color,intensity,snake,number_of_leds);
  snake <<= 1;
  snake |= (snake>>16);

  delay(125); // About 100 ms on slowed-down CPU.
}
