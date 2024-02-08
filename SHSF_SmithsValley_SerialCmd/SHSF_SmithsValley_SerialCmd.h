// all # define options should be defined here,  this keeps them together in a nice way.
//
String VERSION_ID = "SerialCmd V6 2024-Jan-25";
/* 
V6 - Moved to GitHub.
V5 - Added analog input for Accessory (5V) voltage.
V4 - Changed pin definitions for better layout on pcb.
V3 - Removed Bluetooth through SoftwareSerial and now through hardware Serial.
*/
// Values to set in SerialCmd.h configuration.
/*
#define SERIALCMD_FORCEUC    0  // If set to 1 force uppercase for serial command
#define SERIALCMD_MAXCMDNUM  16 // Max Number of Command
#define SERIALCMD_MAXCMDLNG  2  // Max Command Length
#define SERIALCMD_MAXBUFFER  6  // Max Buffer  Length
*/
//
String ROAD_NAME = "SH&SF";
String MODULE_NAME = "Smiths Valley";
//
// from Demo_SerialCmd
#define LED_OFF   LOW
#define LED_ON    HIGH
//
// for communications
const unsigned long COM_BAUD_RATE = 9600;
const unsigned long MP3_BAUD_RATE = 9600;
//
// for Shift Register
const int NUMBER_OF_REGISTERS = 5;// CHANGE THIS VALUE TO THE NUMER OF 74HC595 SHIFT REGISTERS!!!
byte* registerState;// Shift register buffer with size = 8 * NUMBER_OF_REGISTERS
//
// for turnout control
const int NORMAL = 1;
const int REVERSE = 0;
const int CROSSOVER_1 = 4;
const int CROSSOVER_2 = 6;
const int TURNOUT_NUMBER_MIN = 1;
const int TURNOUT_NUMBER_MAX = 16;
const int TURNOUT_REGISTER_START_BIT = 0;
//
// for block control
const int RELAY_OFF = LOW;
const int RELAY_ON = HIGH;
const int BLOCK_NUMBER_MIN = 1;
const int BLOCK_NUMBER_MAX = 8;
const int BLOCK_REGISTER_START_BIT = 16;
//
// for cab control
const int CAB_NUMBER_MIN = 1;
const int CAB_NUMBER_MAX = 8;
const int CAB_REGISTER_START_BIT = 24;
//
// for accessory control
const int ACCESSORY_NUMBER_MIN = 1;
const int ACCESSORY_NUMBER_MAX = 8;
const int ACCESSORY_REGISTER_START_BIT = 32;
//
// for sound control
const int  ALL_ABOARD_CLIP    = 1;
const int  TRAIN_WHISLTE_CLIP = 2;
const int  TRAIN_TRACK_CLIP   = 3;
const int  JUST_WHISTLE_CLIP  = 4;
const int  RAILROAD_CROSSING_CLIP = 5;
const int  RAILROAD_CROSSING_SHORT_CLIP = 6;
const int SOUND_NUMBER_MIN = 1;
const int SOUND_NUMBER_MAX = 6;
//
// for Addressable LEDs
const int ADDRESS_LED_COUNT = 4;
const int ADDRESS_LED_BRIGHTNESS = 40;// max = 255
const int ADDRESS_LED_BLOCK_INDEX = 0;
//
//-------------------- Pin Definitions ---------------------
//        LED_BUILTIN = 13 // Fault Indication.
const int ANALOG_VIN_PIN = A0; // Analog input pin for 12V Vin voltage.
const int ANALOG_ACCS_PWR_PIN = A1; // Analog input pin for 5V Accessory voltage.
const int MP3PLAYER_RX_PIN = A2; // DFPlayer Mini module RX, pin 2.
const int MP3PLAYER_TX_PIN = A3; // DFPlayer Mini module TX, pin 3.
const int MP3PLAYER_BUSY_PIN = 2; // DFPlayer Mini Module digital input, pin 16.
const int SHIFT_REGISTER_DATA_PIN = 6; // 74HC595 Shift Register DS data, pin 14.
const int SHIFT_REGISTER_LATCH_PIN = 5; // 74HC595 Shift Register ST_CP latch, pin 12.
const int SHIFT_REGISTER_CLOCK_PIN = 4; // 74HC595 Shift Register SH_CP clock, pin 11.
const int ADDRESS_LED_PIN = 3; // 5050 LED data pin.
