void handleI2cReceivedData() {
    switch (I2cRxData) {
      case 1: 
        soundWhistle();
        break;
      default:
        mySerCmd.Print (F("ERROR: Urecognized I2C command = "));
        mySerCmd.Print(I2cRxData);
        mySerCmd.Print("\n");
        break;
    }
}
//
//
// This function is called by the Wire library when a I2C message is received.
void I2cReceiveEvent(int numBytesReceived) {
  if (newI2cRxData == false) {
    // copy the data to I2cRxData.
    I2cRxData = Wire.read();
    newI2cRxData = true;
  }
  else {
    // dump the data
    while(Wire.available() > 0) {
      byte c = Wire.read();
    }
  }
}
//
//
void showAddressableLeds (int intPattern) {
  /* Below is an example of how to control the individual LEDs inside the 5050 pacakge.
     strip.setPixelColor(X, strip.Color(R,G,B));
     Where X = the specific LED you want to control.
           R = The intensity of Red   0-255 is a valid number.
           G = The intensity of Green 0-255 is a valid number.
           B = The intensity of Blue  0-255 is a valid number.
     Note- Creating white from RGB for a long period of time will
     damage the individual 5050 LED. Short bursts is fine, but do not keep pure white at high intesity for long.
  */
  switch (intPattern) {
    case 0: // all OFF
      strip.clear();
      strip.show();
      break;
    case 1: // color test pattern
      strip.setPixelColor(0, strip.Color(0, 139, 139));// dark cyan
      strip.setPixelColor(1, strip.Color(255, 69, 0)); // dark orange
      strip.setPixelColor(2, strip.Color(0, 128, 0)); // green
      strip.setPixelColor(3, strip.Color(150, 0, 0));// red
      strip.show();
      break;
    case 2: // rainbow
      rainbow(10);
      break;
    default:
      // if nothing else matches,
      strip.clear(); // Turn OFF all
      strip.show();
      break;
  }
}
//
//
bool chkAccessoryPower (void) {
  // Normal range is >= 4.5 and <= 5.5 volts.
  int val = analogRead(ANALOG_ACCS_PWR_PIN);
  if (val < 575 || val > 703)
  { // Voltage is out of range.
  	mySerCmd.Print(F("Accessory Power is out of range:\n"));
  	mySerCmd.Print(F("1.Please check the fuse!\n"));
  	mySerCmd.Print(F("2.Please recheck the connection!\n"));
	  return true;
  }
  else
  {
	  return false;
  }
}
//
//
void showBlockStatus ( void ) {
  bool value;
  for (int i = ADDRESS_LED_BLOCK_INDEX; i < BLOCK_NUMBER_MAX; i++) {
    value = regReadBit(BLOCK_REGISTER_START_BIT + i);
    if (!value) {
      // turn light RED if block is OFF
      strip.setPixelColor(i, strip.Color(150, 0, 0));// red
    }
    else {
      value = regReadBit(CAB_REGISTER_START_BIT + i);
      if (!value) {
        // turn light GREEN if block is ON and cab A
        strip.setPixelColor(i, strip.Color(0, 128, 0)); // green
      }
      else {
        // turn light ORANGE if block is ON and cab B
        strip.setPixelColor(i, strip.Color(255, 69, 0)); // dark orange
      }
    }
    strip.show();
  }
}
//
/***************************************************************************
  from Dr. Duino Explorer_ATP sketch.

          ---------------rainbow Function-----------------------
   This is from Adafruits famous neopixel library which is awesome.
   Allows the addressable LED's on board to change colors in a rainbow
   type flow.

                                USAGE
    rainbow(10);// wait upto 10ms inbetween changing colors.
****************************************************************************/
void rainbow(int wait) {
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
  for (long firstPixelHue = 0; firstPixelHue < 3 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}
// --------------- Function from janisrove/Arduino-74HC595-shift-registers ---------------
// https://github.com/janisrove/Arduino-74HC595-shift-registers/blob/master/ArduinoLEDsWithShiftRegisters/ArduinoLEDsWithShiftRegisters.ino
//
// 18-Mar-2023 Added ability to toggle the current state to allow a single button to control.
// If toggleState = true, the value of state = does not matter.
//
void regWriteBit(int pin, bool state, bool toggleState) {
  //Determines register
  int reg = pin / 8;
  //Determines pin for actual register
  int actualPin = pin - (8 * reg);

  //Begin session
  digitalWrite(SHIFT_REGISTER_LATCH_PIN, LOW);

  for (int i = 0; i < NUMBER_OF_REGISTERS; i++) {
    //Get actual states for register
    byte* states = &registerState[i];

    //Update state
    if (i == reg) {
      if (toggleState) {
        bitWrite(*states, actualPin, !bitRead(*states, actualPin));
      }
      else {
        bitWrite(*states, actualPin, state);
      }
    }
    //Write
    shiftOut(SHIFT_REGISTER_DATA_PIN, SHIFT_REGISTER_CLOCK_PIN, MSBFIRST, *states);
  }

  //End session
  digitalWrite(SHIFT_REGISTER_LATCH_PIN, HIGH);
}
//
//
// regWriteBuffer() will shift all bytes in registerState[] to the shift register.
// Set the registerState[] byte(s) before calling this routine.
void regWriteBuffer ( void ) {
  //Begin session
  digitalWrite(SHIFT_REGISTER_LATCH_PIN, LOW);
  //
  for (size_t i = 0; i < NUMBER_OF_REGISTERS; i++) {
    //Write
    shiftOut(SHIFT_REGISTER_DATA_PIN, SHIFT_REGISTER_CLOCK_PIN, MSBFIRST, registerState[i]);
  }
  //End session
  digitalWrite(SHIFT_REGISTER_LATCH_PIN, HIGH);
}
//
//
bool regReadBit(int pin) {
  //Determines register
  int reg = pin / 8;
  //Determines pin for actual register
  int actualPin = pin - (8 * reg);


  for (int i = 0; i < NUMBER_OF_REGISTERS; i++) {
    //Read state
    if (i == reg) {
      return bitRead(registerState[i], actualPin);
    }
  }
}
