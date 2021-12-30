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

void draw(uint8_t color, uint8_t intensity, uint8_t number_of_leds)
{
  uint8_t red, green, blue;
  
  if (color&0x01) red = intensity;
  else red = 0;
  if (color&0x02) green = intensity;
  else green = 0;
  if (color&0x04) blue = intensity;
  else blue = 0;

  // Write the data to the LEDs.
  for (uint8_t i=0; i<number_of_leds; i++)
  {
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
  static uint8_t intensity = 255;
  
  draw(color,intensity,number_of_leds);
  
  intensity /= 2; // Next intensity.
  if (intensity==0)
  {
    color <<= 1; // Next color.
    intensity = 255;
    if (color==8)
    {
      // Do a white flash.
      color = 7; // White.
      intensity = 255/3; // Not too bright.
    }
    else if (color>7)
    {
      // Start over.
      color = 1;
    }
  }

  delay(40); // About 50 ms on slowed-down CPU.
}
