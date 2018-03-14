/*
  SD card datalogger

 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 8 (for MKRZero SD: SDCARD_SS_PIN)

 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>

const int chipSelect = 8;

File file;
int count = 0;

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ;
  }


  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
  }
  Serial.println("card initialized.");
  //creating file
  SD.remove("test.csv");
  file = SD.open("test.csv",FILE_WRITE);
  file.close();
  //write data to file
  file = SD.open("test.csv",FILE_WRITE);

}
void loop()
{
  file = SD.open("test.csv",FILE_WRITE);
  if(file && count < 100)
  {
    file.print(count++);
    file.print(",");
    file.print(count++);
    file.print(",");
    file.println(count++);
    Serial.println(count);
    file.close();
  }
}








