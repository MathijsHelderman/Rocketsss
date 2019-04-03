  // Final if debugging mode is on
  #define DEBUG_TVC false
  
  // Finals of the two servos in integers (angles)
  #define MINPITCHSERVORANGE 60
  #define MAXPITCHSERVORANGE 130
  #define MINYAWSERVORANGE 30
  #define MAXYAWSERVORANGE 110
  
  // Finals of the tvc initializing settings
  #define ZONES 5 // (int, ALWAYS ODD NUMBER, incl stopzone) number of zones within the range in which a different tvc degree is instructed
  #define STOPZONEDEGREES 5 // (int, minimum 5 otherwise no tvc) amount of degrees above and below middle, when the rocket is going straight up, where tvc will be off
  #define SPEED 5 // (int)
  
  // Global variables defined by the defines
  float middlePitchServoRange = (MINPITCHSERVORANGE + MAXPITCHSERVORANGE) / 2;
  float middleYawServoRange = (MINYAWSERVORANGE + MAXYAWSERVORANGE) / 2;
  int zonesOneSide = (ZONES - 1) / 2;
  float degreesPitchPerZone = (MAXPITCHSERVORANGE - MINPITCHSERVORANGE - (2*STOPZONEDEGREES)) / (zonesOneSide * 2);
  float degreesYawPerZone = (MAXYAWSERVORANGE - MINYAWSERVORANGE - (2*STOPZONEDEGREES)) / (zonesOneSide * 2);

  // Method testing the tvc
  void  checkTVC() {
    Serial.println("Checking TVC. \r\nTVC activated");
  }
  
  // Determine in which "zone" the rocket is angled within the range of the servo
  // and what the servo should do about it
  int CalculateServoInstruction (int servo) { 
    int instruction = 0;
    switch (servo) {
      case 0: // pitch servo
        switch(CheckRange((middlePitchServoRange + pitch), MINPITCHSERVORANGE, MAXPITCHSERVORANGE)) {
            case 0: instruction = MAXPITCHSERVORANGE; // When the rocket pitches to below the pitch range, counter it with max pitch
            case 1: 
                for (int i = (-1*(zonesOneSide-1)); i >= zonesOneSide; i++) { // Start at the range from MINPITCHSERVORANGE to the first zone 
                  if (i == 0) {
                    continue; // skip i == 0 and go to i == 1
                  }
                  else if (i == -1 || i == 1) { //
                    if (((middlePitchServoRange + pitch) > (middlePitchServoRange + (STOPZONEDEGREES * -1))) &&
                        ((middlePitchServoRange + pitch) < (middlePitchServoRange + (STOPZONEDEGREES * 1)))) {
                      instruction = 0; // Let the servo do nothing, go back to start
                      break; // break out of loop immediately
                    }
                  }
                  else {
                    if (((middlePitchServoRange + pitch) < (middlePitchServoRange + (degreesPitchPerZone * i))) &&
                        ((middlePitchServoRange + pitch) >= (middlePitchServoRange + (degreesPitchPerZone * (i-1))))) {
                      instruction = (middlePitchServoRange + (degreesPitchPerZone * i));
                      break; // break out of loop immediately
                    }
                  }
                }
            case 2: instruction = MINPITCHSERVORANGE; // When the rocket pitches to above the pitch range, counter it with min pitch
          }
        break;
      case 1: // yaw servo
        switch(CheckRange((middleYawServoRange + yaw), MINYAWSERVORANGE, MAXYAWSERVORANGE)) {
            case 0: instruction = MAXYAWSERVORANGE; // When the rocket yaws to below the yaw range, counter it with max yaw
            case 1: 
                for (int i = (-1*(zonesOneSide-1)); i >= zonesOneSide; i++) { // Start at the range from MINPITCHSERVORANGE to the first zone 
                  if (i == 0) {
                    continue; // skip i == 0 and go to i == 1
                  }
                  else if (i == -1 || i == 1) { //
                    if (((middleYawServoRange + pitch) > (middleYawServoRange + (STOPZONEDEGREES * -1))) &&
                        ((middleYawServoRange + pitch) < (middleYawServoRange + (STOPZONEDEGREES * 1)))) {
                      instruction = 0; // Let the servo do nothing, go back to start
                      break; // break out of loop immediately
                    }
                  }
                  else {
                    if (((middleYawServoRange + pitch) < (middleYawServoRange + (degreesYawPerZone * i))) &&
                        ((middleYawServoRange + pitch) >= (middleYawServoRange + (degreesYawPerZone * (i-1))))) {
                      instruction = (middleYawServoRange + (degreesYawPerZone * i));
                      break; // break out of loop immediately
                    }
                  }
                }
            case 2: instruction = MINYAWSERVORANGE; // When the rocket yaws to above the yaw range, counter it with min yaw
        }
      break;
    }
    return instruction;
  }

  int CalculateServoInstruction_WithAcc(int servo) {
    int instruction = 0;
    switch (servo) {
      case 0: // pitch servo
        float pitchAngleV = GetPitchAngleVelocity(20); // get the pitch angle velocity over time in ms 
        
        break;
      case 1: // yaw servo
        float yawAngleV = GetYawAngleVelocity(20); // get the yaw angle velocity over time in ms
        
        break;
    }
  }

  // Determine the angle velocity by meassuring the acceleration over time
  float GetPitchAngleVelocity(int timePeriod) { // timePeriod in ms
    
  }
  
  // Determine the angle velocity by meassuring the acceleration over time
  float GetYawAngleVelocity(int timePeriod) { // timePeriod in ms
    
  }
  
  // Method to check a float number against a integer range
  //   return 0 = lower than the range
  //   return 1 = within range
  //   return 2 = higher than the range
  int CheckRange (float numb, int minRange, int maxRange) {
    if (numb <= minRange) {
      return 0;
    }
    else if (numb >= maxRange) {
      return 2;
    }
    else {
      return 1;
    }
  }

  // Set the servo to the specified instruction
  //    servo 0 is the pitch servo
  //    servo 1 is the yaw servo  
  void SetServo(int servo, int instruc) {
    switch(servo) {
//      case 0: pitchServo.write(instruc); break;
//      case 1: yawServo.write(instruc); break;
    }
  }

  // Set the thrust vector control to certain instructions
  //    while making sure that the instructions are within the
  //    range of the servos.
  //        servo 0 is the pitch servo
  //        servo 1 is the yaw servo
  void SetTVC(void) {
    SetServo(0, CalculateServoInstruction(0));
    // Misschien een kleine delay hiertussen?
    SetServo(1, CalculateServoInstruction(1));
  }
  
  // Returns true if success
  boolean TVCInit() {
    if(CheckTVCDefines()) {
      return true;
    }
    return false;
  }

  // Checks if the defines are correctly defined
  boolean CheckTVCDefines() {
    if (CheckTVCDefinedServoRanges() &&
        CheckTVCDefinedZones() &&
        CheckTVCDefinedStopZone() &&
        CheckTVCDefinedSpeed()) {
            return true;
    }
    return false;
  }

  // Checks if the defined servo ranges are of type int
  boolean CheckTVCDefinedServoRanges() {
    if (MINPITCHSERVORANGE % 1 == 0 &&
        MAXPITCHSERVORANGE % 1 == 0 &&
        MINYAWSERVORANGE % 1 == 0 &&
        MAXYAWSERVORANGE % 1 == 0) { 
          return true;
    }
    return false;
  }

  boolean CheckTVCDefinedZones() {
    if (ZONES % 1 == 0 && ZONES % 2 != 0) {
      return true;
    }
    return false;
  }

  // Check if the stopzone is not outside of the range of the servos
  boolean CheckTVCDefinedStopZone() {
    if (STOPZONEDEGREES % 1 == 0) { // check if angle is of type int
      int adjustedStopZone = STOPZONEDEGREES * 2; // The stopzone angles from the middle in both directions
      if (adjustedStopZone >= MINPITCHSERVORANGE && adjustedStopZone >= MINYAWSERVORANGE &&
              adjustedStopZone < MAXPITCHSERVORANGE && adjustedStopZone < MAXYAWSERVORANGE) {
        return true;
      }
    }
    return false;
  }

  boolean CheckTVCDefinedSpeed() {
    if (SPEED % 1 == 0) {
      return true;
    }
    return false;
  }
