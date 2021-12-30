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

ci_ws2812b_rgb_t rainbow[] = 
{
  {255,   0,   0}, {255,  87,   0}, {205, 255,   0}, {  0, 255,  13},
  {255, 105,   0}, {187, 255,   0}, {  0, 255,  30}, {  0, 255, 248},
  {170, 255,   0}, {  0, 255,  48}, {  0, 243, 255}, {  0,  26, 255},
  {  0, 255,  66}, {  0, 226, 255}, {  0,   8, 255}, {  0,   0, 255}
};

ci_ws2812b_rgb_t box[] = 
{
  {100,0,0}, {100,   0,   0}, {100,   0,   0}, {100,0,0},
  {100,0,0}, {  0, 100,   0}, {  0,   0, 100}, {100,0,0},
  {100,0,0}, {  0,   0, 100}, {  0, 100,   0}, {100,0,0},
  {100,0,0}, {100,   0,   0}, {100,   0,   0}, {100,0,0}
};

ci_ws2812b_rgb_t rainbow2[] = 
{
  {100,   0,   0}, { 50,  25,   0}, { 25,  50,   0}, {   0, 100,   0},
  { 50,  25,   0}, { 50,  25,  25}, { 25,  50,  25}, {   0,  50,  25},
  { 25,  50,   0}, { 25,  50,  25}, { 25,  25,  50}, {   0,  25,  50},
  {  0, 100,   0}, {  0,  50,  25}, {  0,  25,  50}, {   0,   0, 100},
};

// My 4x4 LED matrix is organised like this:
//
// 3  4 11 12
// 2  5 10 13
// 1  6  9 14
// 0  7  8 15
//
// Therefor some remapping is done here.
void show_row(ci_ws2812b_rgb_t row[], bool up)
{
  uint8_t j, step;
  if (up==true)
  {
    j = 0;
    step = 1;
  }
  else
  {
    j = 3;
    step = -1;
  }
  for (uint8_t i=0; i<4; i++)
  {
    LEDs.write(row[j].r,row[j].g,row[j].b);
    j += step;
  }
}

void draw(ci_ws2812b_rgb_t bmp[])
{
  show_row(&bmp[0],true);
  show_row(&bmp[4],false);
  show_row(&bmp[8],true);
  show_row(&bmp[12],false);
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
  
  //draw(rainbow);
  //draw(rainbow2);
  draw(box);
}

void loop(void)
{
}
