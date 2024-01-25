/* Got my start with this!
   https://github.com/gpb01/SerialCmd
   Demo_SerialCmd - A simple program to demostrate the use of SerialCmd
   library to show the capability to receive commands via serial port.

   Copyright (C) 2013 - 2022 Guglielmo Braguglia

   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

   This is free software: you can redistribute it and/or modify it under
   the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This software is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

*/
// ------------------- Functions I Added --------------------
//
void toggleCab ( void ) {
  char *   sParam;
  unsigned long ulngNumber;
  //
  sParam = mySerCmd.ReadNext();
  if ( sParam == NULL ) {
    mySerCmd.Print ( F ( "ERROR: Missing cab number \n" ) );
    return;
  }
  //
  ulngNumber = strtoul(sParam, NULL, 10);
  if ((ulngNumber < CAB_NUMBER_MIN) || (ulngNumber > CAB_NUMBER_MAX)) {
    mySerCmd.Print ( F ( "ERROR: Out of range cab number \n" ) );
    return;
  }
  //
  regWriteBit(ulngNumber - 1 + CAB_REGISTER_START_BIT, 0, true); // cab numbers start at "1"
  //
  showBlockStatus();
  //
  sendOK();
}
//
//
void setCabAll ( void ) {
  char *   sParam;
  bool blnState;
  //
  sParam = mySerCmd.ReadNext();
  if ( sParam == NULL ) {
    mySerCmd.Print ( F ( "ERROR: Missing cab letter a or b \n" ) );
    return;
  }
  //
  if (strcmp(sParam, "b") == 0) {
    if (RELAY_ON == HIGH) {
      blnState = true;
    }
    else {
      blnState = false;
    }
  }
  else if (strcmp(sParam, "a") == 0) {
    if (RELAY_OFF == HIGH) {
      blnState = true;
    }
    else {
      blnState = false;
    }
  }
  else {
    mySerCmd.Print ( F ( "ERROR: Out of range cab letter \n" ) );
    return;
  }
  for (int i = CAB_REGISTER_START_BIT; i < CAB_REGISTER_START_BIT + CAB_NUMBER_MAX; i++) {
    regWriteBit(i, blnState, false);
  }
  regWriteBuffer();
  //
  showBlockStatus();
  //
  sendOK();
}
//
//
void toggleBlock ( void ) {
  char *   sParam;
  unsigned long ulngNumber;
  //
  sParam = mySerCmd.ReadNext();
  if ( sParam == NULL ) {
    mySerCmd.Print ( F ( "ERROR: Missing block number \n" ) );
    return;
  }
  //
  ulngNumber = strtoul(sParam, NULL, 10);
  if ((ulngNumber < BLOCK_NUMBER_MIN) || (ulngNumber > BLOCK_NUMBER_MAX)) {
    mySerCmd.Print ( F ( "ERROR: Out of range block number \n" ) );
    return;
  }
  //
  regWriteBit(ulngNumber - 1 + BLOCK_REGISTER_START_BIT, 0, true); // block numbers start at "1"
  //
  showBlockStatus();
  //
  sendOK();
}
//
//
void setBlockAll ( void ) {
  char *   sParam;
  bool blnState;
  //
  sParam = mySerCmd.ReadNext();
  if ( sParam == NULL ) {
    mySerCmd.Print ( F ( "ERROR: Missing block letter o or f \n" ) );
    return;
  }
  //
  if (strcmp(sParam, "o") == 0) {
    if (RELAY_ON == HIGH) {
      blnState = true;
    }
    else {
      blnState = false;
    }
  }
  else if (strcmp(sParam, "f") == 0) {
    if (RELAY_OFF == HIGH) {
      blnState = true;
    }
    else {
      blnState = false;
    }
  }
  else {
    mySerCmd.Print ( F ( "ERROR: Out of range block letter \n" ) );
    return;
  }
  for (int i = BLOCK_REGISTER_START_BIT; i < BLOCK_REGISTER_START_BIT + BLOCK_NUMBER_MAX; i++) {
    regWriteBit(i, blnState, false);
  }
  regWriteBuffer();
  //
  showBlockStatus();
  //
  sendOK();
}
//
//
void setTurnoutAll ( void ) {
  char *   sParam;
  //
  sParam = mySerCmd.ReadNext();
  if ( sParam == NULL ) {
    mySerCmd.Print ( F ( "ERROR: Missing turnout position \n" ) );
    return;
  }
  //
  if (strcmp(sParam, "n") == 0) {
    if (NORMAL == 1) {
      registerState[0] = 0xFF; // T1 through T8
      registerState[1] = 0xFF; // T9 through T16
    }
    else {
      registerState[0] = 0x00; // T1 through T8
      registerState[1] = 0x00; // T9 through T16
    }
  }
  else if (strcmp(sParam, "r") == 0) {
    if (REVERSE == 1) {
      registerState[0] = 0xFF; // T1 through T8
      registerState[1] = 0xFF; // T9 through T16
    }
    else {
      registerState[0] = 0x00; // T1 through T8
      registerState[1] = 0x00; // T9 through T16
    }
  }
  else {
    mySerCmd.Print ( F ( "ERROR: Out of range turnout position \n" ) );
    return;
  }
  regWriteBuffer();
  sendOK();
}
//
//
void toggleTurnout ( void ) {
  char *   sParam;
  unsigned long ulngNumber;
  //
  sParam = mySerCmd.ReadNext();
  if ( sParam == NULL ) {
    mySerCmd.Print ( F ( "ERROR: Missing turnout number \n" ) );
    return;
  }
  //
  ulngNumber = strtoul(sParam, NULL, 10);
  if ((ulngNumber < TURNOUT_NUMBER_MIN) || (ulngNumber > TURNOUT_NUMBER_MAX)) {
    mySerCmd.Print ( F ( "ERROR: Out of range turnout number \n" ) );
    return;
  }
  //
  regWriteBit(ulngNumber - 1 + TURNOUT_REGISTER_START_BIT, 0, true); // turnout numbers start at "1"
  //
  if (ulngNumber == CROSSOVER_1)
  {
    regWriteBit(CROSSOVER_2 - 1 + TURNOUT_REGISTER_START_BIT, 0, true); // turnout numbers start at "1"
  }
  if (ulngNumber == CROSSOVER_2)
  {
    regWriteBit(CROSSOVER_1 - 1 + TURNOUT_REGISTER_START_BIT, 0, true); // turnout numbers start at "1"
  }
  sendOK();
}
//
//
void playSound (int intSoundClip, int intVolume) {
  // Volume level 0-30 max
  //
  // If you do not want tracks to be interupted, simply wrap the call to the sound clip like this
  if (digitalRead(MP3PLAYER_BUSY_PIN) == HIGH)
  {
    MP3Player.volume(intVolume);
    MP3Player.play(intSoundClip);
    //
    sendOK();
  }
  else
  {
    mySerCmd.Print(F("MP3Player Mini is busy playing another clip.\n"));
  }
}
//
//
void soundTrack (void) {
  playSound(TRAIN_TRACK_CLIP, 10);
}
//
//
void soundWhistle (void) {
  playSound(JUST_WHISTLE_CLIP, 10);
}
//
//
void soundAllAboard (void) {
  playSound(ALL_ABOARD_CLIP, 10);
}
//
//
void moveCementPipeUnloader (void) {
  // Accessory8 = Cement Pipe Unloader
  regWriteBit(7 + ACCESSORY_REGISTER_START_BIT, 1, false); // accessory numbers start at "1", register starts at "0"
  delay(2000);
  regWriteBit(7 + ACCESSORY_REGISTER_START_BIT, 0, false); // accessory numbers start at "1", register starts at "0"
  sendOK();
}
//
//
void dsplyVoltages() {
  int intReading = analogRead(ANALOG_VIN_PIN);
  float fltVinVoltage = (float(intReading) * 5 / 1023) * 3; // 0 to 15 volt range
  //
  intReading = analogRead(ANALOG_ACCS_PWR_PIN);
  float fltAccsVoltage = (float(intReading) * 5 / 1023) * 1.6; // 0 to 8 volt range
  //
  mySerCmd.Print(F("Vin = "));
  mySerCmd.Print(fltVinVoltage);
  mySerCmd.Print(F(" Volts\n"));
  //
  mySerCmd.Print(F("Vaccs = "));
  mySerCmd.Print(fltAccsVoltage);
  mySerCmd.Print(F(" Volts\n"));
}
//
//
void dsplyFaultStatus (void) {
  mySerCmd.Print(F("Accessory Power Fault = "));
  String temp = (blnAccsPwrFault)?"True":"False";
  mySerCmd.Print(temp + char(10));
  //
  mySerCmd.Print(F("MP3 Player Fault = "));
  temp = (blnMp3PlayerFault)?"True":"False";
  mySerCmd.Print(temp + char(10));
}
//
//
void getVersion (void) {
  mySerCmd.Print(F("Arduino:  ") );
  mySerCmd.Print(VERSION_ID + char(10));
}
//
// --------------- User function from Demo_SerialCmd ---------------

void sendOK ( void ) {
  mySerCmd.Print ( F ( "OK \n" ) );
}
//
// --------------- Functions from Demo_SerialCmd ---------------

void set_LEDON ( void ) {
  digitalWrite ( LED_BUILTIN, LED_ON );
}

void set_LEDOF ( void ) {
  digitalWrite ( LED_BUILTIN, LED_OFF );
}
