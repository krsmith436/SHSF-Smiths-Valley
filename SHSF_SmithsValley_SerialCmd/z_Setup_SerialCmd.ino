// --------------- Setup functions ---------------
//
void setupShiftRegister() {
  //
  pinMode(SHIFT_REGISTER_LATCH_PIN, OUTPUT);
  pinMode(SHIFT_REGISTER_CLOCK_PIN, OUTPUT);
  pinMode(SHIFT_REGISTER_DATA_PIN, OUTPUT);
  //
  registerState = new byte[NUMBER_OF_REGISTERS];
  //
  // Set the startup states for the registers
  registerState[0] = 0xFF; // T1 through T8
  registerState[1] = 0xFF; // T9 through T16
  registerState[2] = 0x00; // Block1 through Block8
  registerState[3] = 0x00; // Cab1 through Cab8
  registerState[4] = 0x00; // Accessory1 through Accessory8
  //
  regWriteBuffer();
}
//
//
void setupAddressableLeds () {
  strip.begin(); // INITIALIZE NeoPixel (5050 LED) strip object (REQUIRED)
  mySerCmd.Print(F("Addressable LEDs online.\n"));
  strip.setBrightness(ADDRESS_LED_BRIGHTNESS);
  strip.show();
}
//
//
bool setupMP3Player() {
  pinMode(MP3PLAYER_BUSY_PIN, INPUT);
  serialMP3Player.begin(MP3_BAUD_RATE);
  if (MP3Player.begin(serialMP3Player))
  {
  	mySerCmd.Print(F("MP3Player Mini online.\n"));
    //
    return false;
  }
  else
  {
  	mySerCmd.Print(F("Unable to begin MP3Player Mini:\n"));
  	mySerCmd.Print(F("1.Please recheck the connection!\n"));
  	mySerCmd.Print(F("2.Please insert the SD card!\n"));
    //
  	return true;
  }
}
