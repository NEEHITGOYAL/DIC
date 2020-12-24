#include <SoftwareSerial.h>
#include<SPI.h>
#include<RH_RF95.h>
#include <TinyGPS.h>
RH_RF95 rf95;
TinyGPS gps;
SoftwareSerial ss(4, 3);

void setup()
{
  
  Serial.begin(9600);
  while(!Serial)
  if(!rf95.init())
    Serial.print("init failed");
  ss.begin(9600);
  Serial.print("Simple TinyGPS library v. "); Serial.println(TinyGPS::library_version());
  Serial.println("by Mikal Hart");
  Serial.println();
}

void loop()
{
  float flat,flon;
  char data[100];
  char data2[100];
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.println(flat);
    Serial.print(" LON=");
    Serial.println(flon);
    float flat1= flat;
    float flon1=flon;
    Serial.print(flat1);
    Serial.print(flon1);
    dtostrf(flat1,0,6,data);
    dtostrf(flon1,0,6,data2);
    Serial.print(data);
    strcat(data,",");
    strcat(data,data2);
    Serial.print(data);
    rf95.send(data, sizeof(data));
  gps.stats(&chars, &sentences, &failed);
  
  if (chars == 0)
 00   Serial.println("** No characters received from GPS: check wiring **");
}
