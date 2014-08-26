#include <MatrixText.h>

/********************************************************
/****** 7 Segment LED driver board Basic Code 1 *********
/****** by Matt Little **********************************
/****** Date: 5/4/13 ************************************
/****** info@re-innovation.co.uk ************************
/****** www.re-innovation.co.uk *************************
/********************************************************

/* Adaped by Daniel Swann 26/08/2014 to test MatrixText *

This example uses the Serial Shift output to control a 7 segment LED display
Data is shifted out serially and only when all the shift registers are filled does the latch cause
the LED outputs to update.
The LED boards require 12V supply as they contain strings of 4 LEDs.
The wiring for each board is as follows (P1 and P2):

  Pin 1  ->  Serial LATCH -> A3 on Arduino (in this example)
  Pin 2  ->  Serial CLOCK -> A5 on Arduino (in this example)
  Pin 3  ->  Serial DATA  -> A4 on Arduino (in this example)  
  Pin 4  ->  GROUND       -> GND on Arduino
  Pin 5  ->  +5V          -> +5V on Arduino
  Pin 6  ->  GROUND       -> GND for LED supply
  Pin 7  ->  +12V         -> +12V for LED supply

Use a 0.1uF capacitor between Pin 1 (sLATCH) and Pin 4 (GND) to prevent flicker on the display.

see www.re-innovation.co.uk for more details
 
*/

#define DISPLAY_WIDTH  12
#define DISPLAY_HEIGHT 12

// This is for the serial shifted output data
const int sLatch = 0;   //Pin connected to ST_CP of 74HC595
const int sClk =  1;    //Pin connected to SH_CP of 74HC595
const int sData = 4;    //Pin connected to DS of 74HC595
const int led = 6;  //LED of Minimus
const int swInputA = 7;  // An input switch

int number = 0;  // This will be the displayed data

uint8_t dataArray[DISPLAY_WIDTH];  // This holds the data to display 

MatrixText *mt1;  // MatrixText string

const char text[] = "Nottingham Hackspace";

void setup()
{
  //set pins to output so you can control the shift register
  pinMode(sLatch, OUTPUT);
  pinMode(sClk, OUTPUT);
  pinMode(sData, OUTPUT);
  pinMode(led, OUTPUT);   
  pinMode(swInputA, INPUT_PULLUP); 
  
  mt1 = new MatrixText(set_xy);
  mt1->show_text(text, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
  mt1->set_scroll_speed(100); // Advance text position every 100ms
  
  memset(dataArray,0,sizeof(dataArray));  // Set dataArray to clear it 
}

void loop()
{ 
  mt1->loop(); // do scrolling text
    
  // Write the dataArray to the LED matrix:
  digitalWrite(sLatch, LOW);  
  for(int j=0;j<sizeof(dataArray);j++)
  {     
    //shiftOut(sData, sClk, MSBFIRST, dataArray[j]);
    shiftOut(sData, sClk, LSBFIRST, dataArray[j]);  // Rotated text
  }
  digitalWrite(sLatch, HIGH); 
  
  // This is the main delay and slows everything down a bit.
  delay(30);
  
}

void set_xy (byte x, byte y, byte val)
{
  // The shift register is in the bit format 70123456, so we must move the last bit and push it into the first bit.
  byte actual_y = (y-1)%8;         
 
  if (val)
    dataArray[x] |= 1 << actual_y;    // Set bit
  else
    dataArray[x] &= ~(1 << actual_y); // Clear bit
}

