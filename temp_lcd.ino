/*
The circuit:
 * LCD RS pin to digital pin 8
 * LCD Enable pin to digital pin 9
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int ButtonVoltage = 0;
int ButtonPressed = 0;
int Backlight = 10;
int fadeValue = 255;

String incomingString;

int row = 0;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  
  // Print a message to the LCD.
  lcd.setCursor (0,0);
  lcd.print("Initialized");
  lcd.setCursor (0,0);

  // listen to serial connections
  Serial.begin(9600);
}

void loop() {
  // read a line from the serial interface and insert it to the correct line on the display
  if(Serial.available() > 0) {
    char c = Serial.read();
    Serial.print(c);
    
    if(c == '\n') {
      lcd.print(incomingString);
      incomingString = "";
      if(row == 0) { row = 1; } else { row = 0; }
      lcd.setCursor(0,row);
      Serial.println(row);
    } 
    else {
      incomingString += c;
    }
  }



  ButtonVoltage = analogRead(0);

  ButtonPressed = ButtonVoltage >> 7; //this removes least significant bits to get more stable reads.

  switch (ButtonPressed)
  {
  case 0:
    break; 
  case 1: 
    break;
  case 2:
    fadeValue = fadeValue -5;
    if (fadeValue == 254) 
    {
      fadeValue = 255;
    }
    analogWrite (Backlight, fadeValue);
    delay (100);
    break;
  case 3:
    break;
  case 5:
    break;
  case 7:
    break;
  }
}




