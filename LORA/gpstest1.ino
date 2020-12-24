#include <SoftwareSerial.h>

#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial ss(4, 3);

void setup()
{
  Serial.begin(9600);
  ss.begin(9600);
  Serial.print("Simple TinyGPS library v. "); Serial.println(TinyGPS::library_version());
  Serial.println("by Mikal Hart");
  Serial.println();
}

void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.println(flat,6);
    Serial.print(" LON=");
    Serial.println(flon,6);
  }
  
  gps.stats(&chars, &sentences, &failed);

  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
}
