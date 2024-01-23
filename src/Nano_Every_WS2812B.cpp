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

#include "Arduino.h"
#include "Nano_Every_WS2812B.h"

/*
 * TODO
 * - Slow down CPU only during SPI transfers?
 * - Make SPI interrupt driven?
 */

void CI_WS2812B::begin(void)
{
#ifdef MCU_SLOW_DOWN
  // This library makes the CPU run at about 12 MHz, adjust baud rates and delay times accordingly!
  // To get the WS2812B protocol timing right it is necessary to slow down the CPU a bit.
  uint8_t calibaba = (CLKCTRL.OSC20MCALIBA&0x7f)/2; // Slows down about 25%
  CCP = CCP_IOREG_gc; // Unlock calibration register.
  CLKCTRL.OSC20MCALIBA = calibaba; // Set new calibation value.
  
  //Serial.begin(144000); // About 25% higher to obtain 115200 baud.
  //Serial.println("Core-independent WS2812B driver");
  //Serial.println("This driver makes the CPU run at about 12 MHz.");
  //Serial.println("Adjust baud rates and delay times etc. accordingly.");
#endif /* MCU_SLOW_DOWN */

  SPI_init();
  TCB2_init();
  CCL_init();
}

void CI_WS2812B::write(uint8_t red, uint8_t green, uint8_t blue)
{
  SPI_transfer(green);
  SPI_transfer(red);
  SPI_transfer(blue);
}

void CI_WS2812B::write_string(int number_of_leds, ci_ws2812b_rgb_t led_data[])
{
  cli();
  for (int i=0; i<number_of_leds; i++)
  {
    // Transmit 24-bit RGB color data 8 bits at a time.
    write(led_data[i].r,led_data[i].g,led_data[i].b); // note: write() function will transmit data in (g,r,b) order
  }
  sei();
  delayMicroseconds(100); // Latch the data.
}

void CI_WS2812B::TCB2_init(void)
{
  // This block is for (oscilloscope) debugging only.
  // TCB2-WO does not have a pin on the Nano Every (PC0 or PB4).
  // PB4 is internally routed to TX0 so you should stay away from it, PC0 is unconnected.
  //pinMode(32,OUTPUT); // PC0 output. PC0 is Arduino pin 32. Emulation: ATmega328 and None.
  //DDRC |= 0x01; // PC0 output. Emulation: ATmega328.
  //PORTC.DIRSET = 0x01; // PC0 output. Emulation: None.
  
  //TCB2.CTRLB = TCB_CCMPEN_bm | TCB_CNTMODE_PWM8_gc; // 8-bit PWM with waveform output on pin.
  TCB2.CTRLB = TCB_CNTMODE_PWM8_gc; // 8-bit PWM, no need to enable the waveform output.
  // F_CPU = 16 MHz, F_TCB2 = 1 MHz, 25%.
  TCB2.CCMPL = 15; // 16-1, F = F_CPU/(CCMPL+1)
  TCB2.CCMPH = 4; // 25%
  //TCB2.CTRLA = 1; // Do not enable timer now.
}

void CI_WS2812B::SPI_init(void)
{
  //SPI.begin(); // Enable SPI pins (works). (For debugging only)
  // Why doesn't this work instead of SPI.begin()?
  //PORTMUX.TWISPIROUTEA = 2; // SPI on PE[3:0]
  //pinMode(11,OUTPUT); // 11 = MOSI
  //pinMode(13,OUTPUT); // 13 = SCK
  
  // F_CPU = 16 MHz, div by 16 (F_SPI = 1 MHz), MSB first, master (host), enabled.
  SPI0.CTRLA = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_PRESC_DIV16_gc;
  SPI0.CTRLB = SPI_MODE_1_gc | SPI_SSD_bm;  // Slave Select disabled.
}

void CI_WS2812B::SPI_transfer(uint8_t data)
{
  // Clear the Write Collision flag, to allow writing
  SPI0.INTFLAGS = SPI0_INTFLAGS;

  TCB2.CNT = -9; // Tweak factor. Adjust count to position waveform correctly with respect to SPI clock.
  TCB2.CTRLA = 1; // Start TCB2.

  SPI0.DATA = data; // Start SPI

  // Wait for transfer to complete
  while ((SPI0.INTFLAGS & SPI_RXCIF_bm)==0)
  {
  }

  TCB2.CTRLA = 0; // Stop TCB2.
}

void CI_WS2812B::CCL_init(void)
{
  // LUT port connections:
  //       LUT0   LUT1   LUT2   LUT3 
  // in0:   PA0    PC0    PD0    PF0
  // in1:   PA1    PC1    PD1    PF1
  // in2:   PA2    PC2    PD2    PF2
  // out:  PA3/6  PC3/6  PD3/6  PF3/6
  
  // LUT1 alternate output is on PC6, which is D4 on Arduino Nano Every.
  PORTMUX.CCLROUTEA |= PORTMUX_LUT1_bm; // Alternate pin for LUT1.

  // Enable Protected register, peripheral must be disabled (ENABLE=0, in TCD.LUT0CTRLA).

  // Use LUT2 to allow routing of TCB2 to LUT1's input0.
  // Configure LUT2 inputs: in0 = MASK, in1 = MASK, in2 = TCB2.
  CCL.LUT2CTRLB = CCL_INSEL0_MASK_gc | CCL_INSEL1_MASK_gc; // in0 = NC, in1 = NC.
  CCL.LUT2CTRLC = CCL_INSEL2_TCB2_gc; // in2 = TCB2.
  CCL.TRUTH2 = 0x10; // The output of LUT2 is 1 when its inputs are 'b100.
  //CCL.LUT2CTRLA = CCL_ENABLE_bm | CCL_OUTEN_bm; // Enable LUT2 and its output PD3 (= A0).
  CCL.LUT2CTRLA = CCL_ENABLE_bm; // Enable LUT2, output not needed.

  // Configure LUT1 inputs.
  // (ATmega4809 CCL appears to have two SPI MOSI connections but no MISO?)
  // in0 = LUT2, in1 = SPI MOSI, in2 = SPI SCK
  CCL.LUT1CTRLB = CCL_INSEL0_LINK_gc | CCL_INSEL1_SPI0_gc; // in0 = LINK LUT2, in1 = SPI0 MOSI.
  CCL.LUT1CTRLC = CCL_INSEL2_SPI0_gc; // in2 = SPI0 SCK.

  // Configure LUT truth table.
  //     in2     in1    in0
  // A = SCK &  MOSI & nTCB -> 'b110 -> TRUTHn[6]=1 -> 0x20
  // B = SCK & nMOSI &  TCB -> 'b101 -> TRUTHn[5]=1 -> 0x40
  // C = SCK &  MOSI &  TCB -> 'b111 -> TRUTHn[7]=1 -> 0x80
  // LUT out = A + B + C = 0x20 + 0x40 + 0x80 = 0xe0
  CCL.TRUTH1 = 0xe0; // The output of LUT1 is 1 when its inputs are 'b111, 'b101 or 'b110.

  // Enable LUT, enable output pin, CLK_PER is clocking the LUT.
  CCL.LUT1CTRLA = CCL_ENABLE_bm | CCL_OUTEN_bm; // Enable LUT1 and its output PC6 (= D4).
  
  CCL.CTRLA = CCL_ENABLE_bm | CCL_RUNSTDBY_bm; // Enable CCL & run in standby.
}
