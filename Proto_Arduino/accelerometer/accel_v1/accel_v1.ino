#include "SparkFun_LIS331.h"
#include <SPI.h>

LIS331 xl;

void setup() 
{
  // put your setup code here, to run once:
  pinMode(9,INPUT);       // Interrupt pin input
  pinMode(10, OUTPUT);    // CS for SPI
  digitalWrite(10, HIGH); // Make CS high
  pinMode(11, OUTPUT);    // MOSI for SPI
  pinMode(12, INPUT);     // MISO for SPI
  pinMode(13, OUTPUT);    // SCK for SPI
  SPI.begin();
  xl.setSPICSPin(10);     // This MUST be called BEFORE .begin() so 
                          //  .begin() can communicate with the chip
  xl.begin(LIS331::USE_SPI); // Selects the bus to be used and sets
                          //  the power up bit on the accelerometer.
                          //  Also zeroes out all accelerometer
                          //  registers that are user writable.
  // This next section configures an interrupt. It will cause pin
  //  INT1 on the accelerometer to go high when the absolute value
  //  of the reading on the Z-axis exceeds a certain level for a
  //  certain number of samples.

  Serial.begin(115200);
}

void loop() 
{
  static long loopTimer = 0;
  int16_t x, y, z;
  if (millis() - loopTimer > 1000)
  {
    loopTimer = millis();
    xl.readAxes(x, y, z);  // The readAxes() function transfers the
                           //  current axis readings into the three
                           //  parameter variables passed to it.
    Serial.println(x);
    Serial.println(y);
    Serial.println(z);
    Serial.println(xl.convertToG(100,x)); // The convertToG() function
    Serial.println(xl.convertToG(100,y)); // accepts as parameters the
    Serial.println(xl.convertToG(100,z)); // raw value and the current
    Serial.println(" ");                // maximum g-rating.
  }

}
