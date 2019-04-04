
void writeToSD(void)
{  
  // Get the data
  String dataString = createIMUDataString();
    
  // Open the file. Note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(FILENAME, FILE_WRITE);
  // Check if the file is available, if so, write to it.
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  else {
    Serial.println("ERROR: cannot open the file: " + FILENAME);
  }       
}

String createIMUDataString() {
  String dataString = "";
  dataString += ToDeg(roll);
  dataString += ", ";
  dataString += ToDeg(pitch);
  dataString += ", ";
  dataString += ToDeg(yaw);
  dataString += ", ";
  dataString += String(AN[3]);
  dataString += ", ";
  dataString += String(AN[4]);
  dataString += ", ";
  dataString += String(AN[5]);
  dataString += ", ";
  dataString += String(altitude-init_alt);
  dataString += ", ";
  dataString += (millis());
  return dataString;
}
