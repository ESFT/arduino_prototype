
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include <SD.h>
#include "TimerOne.h"

LSM9DS1 imu;

#define LSM9DS1_M 0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

#define PRINT_CALCULATED
#define PRINT_SPEED 250 // 250 ms between prints
static unsigned long lastPrint = 0; // Keep track of print time

#define DECLINATION -8.58 // Declination (degrees) in Boulder, CO.

File file;

void setup() 
{  
  //Timer1.initialize(1000000);
  //Timer1.attachInterrupt(logData);
  
  Serial.begin(115200);

  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;

  if (!imu.begin())
  {
    Serial.println("Failed to communicate with LSM9DS1.");
    Serial.println("Double-check wiring.");
    Serial.println("Default settings in this sketch will " \
                  "work for an out of the box LSM9DS1 " \
                  "Breakout, but may need to be modified " \
                  "if the board jumpers are.");
    while (1)
      ;
  }
  
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);

  if (!SD.begin(8)) {
    Serial.println("Card failed, or not present");
  }
  Serial.println("card initialized.");
  //creating file
  SD.remove("imuLog.csv");
  file = SD.open("imuLog.csv",FILE_WRITE);
  file.close();
  //write data to file
  file = SD.open("imuLog.csv",FILE_WRITE);
  if(file)
  {
    file.println("NUM, Gx, Gy, Gz, Ax, Ay, Az, Mx, My, Mz, Pitch, Roll, Heading");
    file.close();
  }

}

void loop()
{
  logData();
}

void logData()
{ 
  file = SD.open("imuLog.csv", FILE_WRITE);
  if(file)
  {
    printIMU();
    file.close();
  }
}

void printIMU()
{
  static int num = 0;
  if ( imu.gyroAvailable() )
  {
    imu.readGyro();
  }
  if ( imu.accelAvailable() )
  {
    imu.readAccel();
  }
  if ( imu.magAvailable() )
  {
    imu.readMag();
  }
  
  if ((lastPrint + PRINT_SPEED) < millis())
  {
    file.print(num++);
    file.print(",");
    printGyro();  // Print "G: gx, gy, gz"
    printAccel(); // Print "A: ax, ay, az"
    printMag();   // Print "M: mx, my, mz"
    printAttitude(imu.ax, imu.ay, imu.az, 
                 -imu.my, -imu.mx, imu.mz);
    //Serial.println();
    
    lastPrint = millis(); // Update lastPrint time
  }
}

void printGyro()
{
  //Serial.print("G: ");
#ifdef PRINT_CALCULATED
  file.print(imu.calcGyro(imu.gx), 2);
  file.print(", ");
  file.print(imu.calcGyro(imu.gy), 2);
  file.print(", ");
  file.print(imu.calcGyro(imu.gz), 2);
  file.print(", ");
  //Serial.println(" deg/s");
#elif defined PRINT_RAW
  Serial.print(imu.gx);
  Serial.print(", ");
  Serial.print(imu.gy);
  Serial.print(", ");
  Serial.print(imu.gz);
#endif
}

void printAccel()
{  
  //Serial.print("A: ");
#ifdef PRINT_CALCULATED
  file.print(imu.calcAccel(imu.ax), 2);
  file.print(", ");
  file.print(imu.calcAccel(imu.ay), 2);
  file.print(", ");
  file.print(imu.calcAccel(imu.az), 2);
  file.print(", ");
  //Serial.println(" g");
#elif defined PRINT_RAW 
  Serial.print(imu.ax);
  Serial.print(", ");
  Serial.print(imu.ay);
  Serial.print(", ");
  Serial.print(imu.az);
#endif

}

void printMag()
{  
  //Serial.print("M: ");
#ifdef PRINT_CALCULATED
  file.print(imu.calcMag(imu.mx), 2);
  file.print(", ");
  file.print(imu.calcMag(imu.my), 2);
  file.print(", ");
  file.print(imu.calcMag(imu.mz), 2);
  file.print(", ");
  //Serial.println(" gauss");
#elif defined PRINT_RAW
  Serial.print(imu.mx);
  Serial.print(", ");
  Serial.print(imu.my);
  Serial.print(", ");
  Serial.print(imu.mz);
#endif
}

void printAttitude(float ax, float ay, float az, float mx, float my, float mz)
{
  float roll = atan2(ay, az);
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));
  
  float heading;
  if (my == 0)
    heading = (mx < 0) ? PI : 0;
  else
    heading = atan2(mx, my);
    
  heading -= DECLINATION * PI / 180;
  
  if (heading > PI) heading -= (2 * PI);
  else if (heading < -PI) heading += (2 * PI);
  else if (heading < 0) heading += 2 * PI;
  
  // Convert everything from radians to degrees:
  heading *= 180.0 / PI;
  pitch *= 180.0 / PI;
  roll  *= 180.0 / PI;
  
  //Serial.print("Pitch, Roll: ");
  file.print(pitch, 2);
  file.print(", ");
  file.print(roll, 2);
  file.print(", ");
  //Serial.print("Heading: "); 
  file.println(heading, 2);
}
