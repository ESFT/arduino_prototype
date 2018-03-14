#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(10, 11); // RX, TX (TX not used)
const int sentenceSize = 80;

char sentence[sentenceSize];

void setup()
{
  Serial.begin(9600);
  gpsSerial.begin(9600);
}

void loop()
{
  getGPS();
}

void getGPS()
{
  static int i = 0;
  if (gpsSerial.available())
  {
    char ch = gpsSerial.read();
    if (ch != '\n' && i < sentenceSize)
    {
      sentence[i] = ch;
      i++;
    }
    else
    {
     sentence[i] = '\0';
     i = 0;
     displayGPS();
    }
  } 
}

void displayGPS()
{
  char field[20];
  char num[20];
  char dir[20];
  float f;
  getField(field, 0);
  if (strcmp(field, "$GPRMC") == 0)
  {
    Serial.print("Lat: ");
    getField(field, 3);  // number
    f = (float)atof(field);
    getField(field, 4); // N/S
    strcpy(dir, field);
    
    if(dir[0] == 'S')
    {
      Serial.print('-');
      Serial.print(f/100);
      Serial.print(dir);
    }
    else
    {
      Serial.print(f/100);
      Serial.print(dir[0]);
    }
    
    Serial.print(" Long: ");
    getField(field, 5);  // number
    f = (float)atof(field);
    getField(field, 6);  // E/W
    strcpy(dir, field);
    
    if(dir[0] == 'W')
    {
      Serial.print('-');
      Serial.print(f/100);
      Serial.println(dir);
    }
    else
    {
      Serial.print(f/100);
      Serial.println(dir[0]);
    }
  }
}

void getField(char* buffer, int index)
{
  int sentencePos = 0;
  int fieldPos = 0;
  int commaCount = 0;
  while (sentencePos < sentenceSize)
  {
    if (sentence[sentencePos] == ',')
    {
      commaCount ++;
      sentencePos ++;
    }
    if (commaCount == index)
    {
      buffer[fieldPos] = sentence[sentencePos];
      fieldPos ++;
    }
    sentencePos ++;
  }
  buffer[fieldPos] = '\0';
} 

/*
    Serial.print(" Long: ");
    getField(field, 5);  // number
    Serial.print(field);
    getField(field, 6);  // E/W
    Serial.print('-');
    Serial.println(field);
 */

