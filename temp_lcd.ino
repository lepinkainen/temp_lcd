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
#include <OneWire.h>

OneWire  ds(2);  // pin 2

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
  lcd.setCursor(0,0);
  
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }
  
  for( i = 0; i < 8; i++) {
    lcd.print(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      lcd.print("CRC is not valid!");
      return;
  }
 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      type_s = 1;
      break;
    case 0x28:
      type_s = 0;
      break;
    case 0x22:
      type_s = 0;
      break;
    default:
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad
  
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  
  // convert the data to actual temperature
  unsigned int raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // count remain gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw << 3;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw << 2; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw << 1; // 11 bit res, 375 ms
    // default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  lcd.setCursor(0,1);
  Serial.println(celsius);
  lcd.print(celsius);
  lcd.print(" C");

  ButtonVoltage = analogRead(0);
  ButtonPressed = ButtonVoltage >> 7; //this removes least significant bits to get more stable reads.

  switch (ButtonPressed)
  {
  case 0: // right
   Serial.println("right");
    break; 
  case 1: // up
   Serial.println("up");
   break;
  case 2: // down
  Serial.println("down");
    break;
  case 3: // left
  Serial.println("left");
    break;
  case 5: // select
  Serial.println("select");
    break;
  case 7: // nothing pressed
  Serial.println("none");
    break;
  }
  
  delay(1000);
}




