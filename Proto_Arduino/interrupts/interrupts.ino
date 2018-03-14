#include "TimerOne.h"

int counter = 0;

void setup() {
  // put your setup code here, to run once:
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(printHello);

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void printHello()
{
  Serial.print("Testing:");
  Serial.println(counter);
  counter++;
}

