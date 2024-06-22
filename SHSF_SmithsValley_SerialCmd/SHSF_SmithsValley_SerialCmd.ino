/***************************************************************************
  Sketch Name: SHSF_SmithsValley
  Written By: Kevin R. Smith
  Created: 2023-Apr-07

  This sketch is part of a DC control system for the Smith, Huotari & Santa Fe (SHSF) HO scale model railroad.
  Elements of control:
  1) 16 Tortoise Slow Motion Switch Machine
      NOTE: Unique feature is the motor can remain powered after movement so no need to turn off.
  2) 8 Blocks with A or B cab control
  3) Sound
  4) Use Built-in LED for Fault Indication

  Hardware used:
  1) Arduino Nano Clone from Dr. Duino Model Railroad Explorer Edition
  2) MLT-BT05 Bluetooth Low Energy (BLE) module with chip (CC2540/CC2541)
  3) 74HC595 Serial to Parallel Shift Register
  CODE:https://github.com/janisrove/Arduino-74HC595-shift-registers/blob/master/ArduinoLEDsWithShiftRegisters/ArduinoLEDsWithShiftRegisters.ino
  4) CD4041 Inverter
  5) DFPlayer Mini Module (MP3 player)
  6) Addressable LEDs for block indication
****************************************************************************
*/
//-----------------Calling libraries needed to run-----------//
#include <Wire.h> // I2C
#include <SoftwareSerial.h> // MP3 player
#include <DFRobotDFPlayerMini.h> // MP3 player functions
#include "SHSF_SmithsValley_SerialCmd.h"
#include <SerialCmd.h> // command line interface
#include <Adafruit_NeoPixel.h> // Addressable LEDs (5050 LED)
//
//------------------Object Instantiation------------------//
SerialCmd mySerCmd ( Serial, SERIALCMD_CR, SERIALCMD_SPACE );// Initialize the SerialCmd constructor
SoftwareSerial serialMP3Player(MP3PLAYER_RX_PIN, MP3PLAYER_TX_PIN); //RX, TX
DFRobotDFPlayerMini MP3Player;
Adafruit_NeoPixel strip(ADDRESS_LED_COUNT, ADDRESS_LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
//
//-----------------GLOBAL VARIABLES-------------------//
// Fault detection
bool blnAccsPwrFault = true;
bool blnMp3PlayerFault = true;
//
// I2C
byte I2cRxData = 0xFF;
volatile bool newI2cRxData = false;
//
//-----------------Setup Code-------------------------//
void setup() {
  Serial.begin(COM_BAUD_RATE);// Initialize the USB port for 9600 baud.
  // set up I2C
  Wire.begin(THIS_DEVICE_I2C_ADDR); // join i2c bus
  Wire.onReceive(I2cReceiveEvent); // register event
  //
  mySerCmd.Print(ROAD_NAME + " - " + MODULE_NAME + char(10));
  mySerCmd.Print(F("Starting setup.\n"));
  //
  pinMode(LED_BUILTIN, OUTPUT);// initialize LED_BUILTIN, using as Fault Indicator.
  set_LEDON(); // turn ON Fault Indicator as a visual indication of a reset.
  //
  dsplyVoltages();
  //
  setupShiftRegister(); //Initialize array for shift register and startup values.
  //
  // These commands are present regardless of a fault.
  mySerCmd.AddCmd ( F ( "f" ) , SERIALCMD_FROMALL, dsplyFaultStatus );
  mySerCmd.AddCmd ( F ( "t" ) , SERIALCMD_FROMALL, toggleTurnout );
  mySerCmd.AddCmd ( F ( "ta" ) , SERIALCMD_FROMALL, setTurnoutAll );
  mySerCmd.AddCmd ( F ( "v" ) , SERIALCMD_FROMALL, dsplyVoltages );
  mySerCmd.AddCmd ( F ( "vn" ) , SERIALCMD_FROMALL, getVersion );
  mySerCmd.AddCmd ( F ( "?" ) , SERIALCMD_FROMALL, getHelp );
  //
  // Check 5V accessory power and do not add any more commands if out of range.
  blnAccsPwrFault = chkAccessoryPower();
  if (!blnAccsPwrFault)
  { // Normal operation.
	  setupAddressableLeds(); // Initialize addressable LEDs.
    //
    showBlockStatus();
    //
    mySerCmd.AddCmd ( F ( "b" ) , SERIALCMD_FROMALL, toggleBlock );
    mySerCmd.AddCmd ( F ( "ba" ) , SERIALCMD_FROMALL, setBlockAll );
    mySerCmd.AddCmd ( F ( "c" ) , SERIALCMD_FROMALL, toggleCab );
    mySerCmd.AddCmd ( F ( "ca" ) , SERIALCMD_FROMALL, setCabAll );
    mySerCmd.AddCmd ( F ( "mp" ) , SERIALCMD_FROMALL, moveCementPipeUnloader );
    //
    blnMp3PlayerFault = setupMP3Player(); // Initialize the DFPlayer Mini Module.
    if (!blnMp3PlayerFault)
    { // Normal operation.
      mySerCmd.AddCmd ( F ( "a" ) , SERIALCMD_FROMALL, soundAllAboard );
      mySerCmd.AddCmd ( F ( "w" ) , SERIALCMD_FROMALL, soundWhistle );
      mySerCmd.AddCmd ( F ( "h" ) , SERIALCMD_FROMALL, soundHorn );
      mySerCmd.AddCmd ( F ( "cc" ) , SERIALCMD_FROMALL, soundTrack );
    }
  }
  // Turn OFF Fault Indicator if no faults detected.
  if ((!blnAccsPwrFault) && (!blnMp3PlayerFault))
  {
    set_LEDOF();
  }
  //
  mySerCmd.Print(F("Setup is complete.\n"));
  //
  getHelp(); // draw a menu
  //
}
// ----------------------------------------------------------------
void getHelp() {
  mySerCmd.Print(ROAD_NAME + " - " + MODULE_NAME + char(10));
  mySerCmd.Print(F("t n\t\tToggle turnout\n"));
  mySerCmd.Print(F("ta n|r\tSet all turnouts\n"));
  if (!blnAccsPwrFault)
  {
    mySerCmd.Print(F("b n\t\tToggle block\n"));
    mySerCmd.Print(F("ba o|f\tSet all blocks\n"));
    mySerCmd.Print(F("c n\t\tToggle cab\n"));
    mySerCmd.Print(F("ca a|b\tSet all cabs\n"));
    mySerCmd.Print(F("mp \t Move pipe unloader\n"));
  }
  if (!blnMp3PlayerFault)
  {
    mySerCmd.Print(F("a \t All Abord sound clip\n"));
    mySerCmd.Print(F("h \t Diesel Horn sound clip\n"));
    mySerCmd.Print(F("w \t Whistle sound clip\n"));
    mySerCmd.Print(F("cc \t Click clack train track sound clip\n"));
  }
  mySerCmd.Print(F("v \t Display Vin and Vaccs values\n"));
  mySerCmd.Print(F("f \t Fault Status Report\n"));
  mySerCmd.Print(F("vn \t Version number of software\n"));
  mySerCmd.Print(F("? \t Help\n"));
}
//-----------------Loop Code-------------------------//
void loop() {
  //
  blnAccsPwrFault = chkAccessoryPower();
  if (!blnAccsPwrFault)
  { // Normal operation.
    if(!blnMp3PlayerFault)
    {
      set_LEDOF(); // Turn OFF Fault Indicator
    }
    // Start of code from Demo_SerialCmd
    int8_t ret;
    ret = mySerCmd.ReadSer();
    if ( ret == 0 )
      mySerCmd.Print (F("ERROR: Urecognized command. \n"));
    // End of code from Demo_SerialCmd
    //
    // Start of code to check if a I2C message has been received.
    if (newI2cRxData == true) {
        handleI2cReceivedData();
        newI2cRxData = false;
    }
    // End of code to check if a I2C message has been received.
  }
  else
  { // Fault condition.
    set_LEDON(); // turn ON Fault Indicator
    dsplyFaultStatus();
    mySerCmd.Print(char(10));
    dsplyVoltages();
    delay(5000);
  }
}
