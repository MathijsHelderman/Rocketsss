// Include libraries:
#include <SPI.h>
#include <SD.h>

// FINALS
#define DEBUG_SD false
const String FILENAME = "datalog.txt";
const int CHIPSELECT = 7;

/*
  SD card test
*/    
  void checkSD() {
    // set up variables using the SD utility library functions:
    Sd2Card card;
    SdVolume volume;
    SdFile root;

    Serial.println("\nInitializing SD card...");

    // we'll use the initialization code from the utility libraries
    // since we're just testing if the card is working!
    if (!card.init(SPI_HALF_SPEED, CHIPSELECT)) {
      Serial.println("\nInitialization failed. Things to check:");
      Serial.println("* is a card inserted?");
      Serial.println("* is your wiring correct?");
      Serial.println("* did you change the CHIPSELECT pin to match your shield or module?");
      while (1);
    } else {
      Serial.println("");
      Serial.println("Wiring is correct and a card is present.");
      Serial.println("");
    }
  
    // print the type of card
    Serial.println();
    Serial.print("Card type:         ");
    switch (card.type()) {
      case SD_CARD_TYPE_SD1:
        Serial.println("SD1");
        break;
      case SD_CARD_TYPE_SD2:
        Serial.println("SD2");
        break;
      case SD_CARD_TYPE_SDHC:
        Serial.println("SDHC");
        break;
      default:
        Serial.println("Unknown");
    }
  
    // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
    if (!volume.init(card)) {
      Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
      while (1);
    }
  
    Serial.print("Clusters:          ");
    Serial.println(volume.clusterCount());
    Serial.print("Blocks x Cluster:  ");
    Serial.println(volume.blocksPerCluster());
  
    Serial.print("Total Blocks:      ");
    Serial.println(volume.blocksPerCluster() * volume.clusterCount());
    Serial.println();
  
    // print the type and size of the first FAT-type volume
    uint32_t volumesize;
    Serial.print("Volume type is:    FAT");
    Serial.println(volume.fatType(), DEC);
  
    volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
    volumesize *= volume.clusterCount();       // we'll have a lot of clusters
    volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
    Serial.print("Volume size (Kb):  ");
    Serial.println(volumesize);
    Serial.print("Volume size (Mb):  ");
    volumesize /= 1024;
    Serial.println(volumesize);
    Serial.print("Volume size (Gb):  ");
    Serial.println((float)volumesize / 1024.0);
  
    Serial.println("\nFiles found on the card (name, date and size in bytes): ");
    root.openRoot(volume);
  
    // list all files in the card with date and size
    root.ls(LS_R | LS_DATE | LS_SIZE);

    /*
     * After the SD card check, check if you can create a file and write and read to/from it
     */
    testSDFileCreateAndDelete();
    testSDWriteAndRead();
  }

/*
  SD card test file creation and deletion
 */
  void testSDFileCreateAndDelete() {
    if (!SD.begin(CHIPSELECT)) {
      Serial.println("initialization failed!");
      while (1);
    }
    Serial.println("initialization done.");
  
    if (SD.exists("example.txt")) {
      Serial.println("example.txt exists.");
    } else {
      Serial.println("example.txt doesn't exist.");
    }
  
    // open a new file and immediately close it:
    Serial.println("Creating example.txt...");
    File myFile = SD.open("example.txt", FILE_WRITE);
    myFile.close();
  
    // Check to see if the file exists:
    if (SD.exists("example.txt")) {
      Serial.println("example.txt exists.");
    } else {
      Serial.println("example.txt doesn't exist.");
    }
  
    // delete the file:
    Serial.println("Removing example.txt...");
    SD.remove("example.txt");
  
    if (SD.exists("example.txt")) {
      Serial.println("example.txt exists.");
    } else {
      Serial.println("example.txt doesn't exist.");
    }
  }

/*
SD card test write/read
*/
  void testSDWriteAndRead() {
    if (!SD.begin(CHIPSELECT)) {
      Serial.println("initialization failed!");
      while (1);
    }
    Serial.println("initialization done.");
  
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File myFile = SD.open("test.txt", FILE_WRITE);
  
    // if the file opened okay, write to it:
    if (myFile) {
      Serial.print("Writing to test.txt...");
      myFile.println("testing 1, 2, 3.");
      // close the file:
      myFile.close();
      Serial.println("done.");
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening test.txt");
    }
  
    // re-open the file for reading:
    myFile = SD.open("test.txt");
    if (myFile) {
      Serial.println("test.txt:");
  
      // read from the file until there's nothing else in it:
      while (myFile.available()) {
        Serial.write(myFile.read());
      }
      // close the file:
      myFile.close();
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening test.txt");
    }
  }

/*
 * Actual SD card methods after the SD card tests
 */
/*
  SD card datalogger
 */
  void writeToSD(String dataString) {
    // see if the card is present and can be initialized:
    if (!SD.begin(CHIPSELECT)) {
      Serial.println("Card failed, or not present");
      // don't do anything more:
      while (1);
    }
    Serial.println("card initialized.");
    
    // make a string for assembling the data to log:
    #if DEBUG
    dataString = "Bla";
    #endif
  
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open(FILENAME, FILE_WRITE);
  
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening " + FILENAME + ".");
    }
  }

/*
  SD card read and print to serial
 */
  void readSDToSerial() {
    // see if the card is present and can be initialized:
    if (!SD.begin(CHIPSELECT)) {
      Serial.println("Card failed, or not present");
      // don't do anything more:
      while (1);
    }
    Serial.println("card initialized.");
  
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open(FILENAME);
  
    // if the file is available, read from it:
    if (dataFile) {
      while (dataFile.available()) {
        Serial.write(dataFile.read());
      }
      dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening " + FILENAME + ".");
    }
  }

/*
 * 
 * Method overload on writeToSD(void)
 *    use this method to write data from the imu to the sd card 
 */
void writeToSD(void)
{  
  Serial.println("");
  // See if the card is present and can be initialized.
  if (!SD.begin(CHIPSELECT)) {
    Serial.println("ERROR: card failed, or not present.");
    // Don't do anything else.
    while (1);
  }
  Serial.println("Card initialized."); 

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
  #if PRINT_EULER == 1
    dataString += "EULER:   ";
    dataString += "ANG: ";
    dataString += ToDeg(roll);
    dataString += ", ";
    dataString += ToDeg(pitch);
    dataString += ", ";
    dataString += ToDeg(yaw);
    dataString += "\r\n";
  #endif   
  #if PRINT_ANALOGS == 1
    dataString += "ANALOGS: ";
    dataString += ",AN:";
    dataString += (AN[0]);  //(int)read_adc(0)
    dataString += (",");
    dataString += (AN[1]);
    dataString += (",");
    dataString += (AN[2]);  
    dataString += (",");
    dataString += (AN[3]);
    dataString +=  (",");
    dataString += (AN[4]);
    dataString +=  (",");
    dataString += (AN[5]);
    dataString += (",");
    dataString += (c_magnetom_x);
    dataString +=  (",");
    dataString += (c_magnetom_y);
    dataString +=  (",");
    dataString += (c_magnetom_z);
    dataString += "\r\n";
  #endif
  #if PRINT_DCM == 1
    dataString += "DCM:     ";
    dataString +=  (",DCM:");
    dataString += (DCM_Matrix[0][0]);
    dataString +=  (",");
    dataString += (DCM_Matrix[0][1]);
    dataString +=  (",");
    dataString += (DCM_Matrix[0][2]);
    dataString +=  (",");
    dataString += (DCM_Matrix[1][0]);
    dataString +=  (",");
    dataString += (DCM_Matrix[1][1]);
    dataString +=  (",");
    dataString += (DCM_Matrix[1][2]);
    dataString +=  (",");
    dataString += (DCM_Matrix[2][0]);
    dataString +=  (",");
    dataString += (DCM_Matrix[2][1]);
    dataString +=  (",");
    dataString += (DCM_Matrix[2][2]);
    dataString += "\r\n";
  #endif
  dataString += "\r\n";
}
