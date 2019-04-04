//CLOCKWISE SERVO IS NEGATIVE DEGREES
//RANGES LOWER: 110 - 80 96 UPPER: 110-60 82

void set_TVC(){
  if (abs(ToDeg(roll))<5){
    lowerServo.write(96);
    }
  if (abs(ToDeg(pitch))<5){
    upperServo.write(82);
    }
  if (ToDeg(roll)>5){
    lowerServo.write(102);
    }
  if (ToDeg(roll)>20){
    lowerServo.write(110);
    }
  if (ToDeg(roll)<-5){
    lowerServo.write(90);
    }
  if (ToDeg(roll)<-20){
    lowerServo.write(80);
    }
  if (ToDeg(pitch)>5){
    upperServo.write(90);
  }
  if (ToDeg(pitch)>20){
    upperServo.write(110);
  }
  if (ToDeg(pitch)<-5){
    upperServo.write(70);
  }
  if (ToDeg(pitch)<-20){
    upperServo.write(60);
  }
}
