#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include<ArduinoJson.h>
#include "DHT.h"
#define RL 20
#define RL135 5
#define RL3 200
#define ro135 52.86//89.41
#define ro4 14.08//32.19
#define ro3 1246.2//70.63
bool my_status;

// IMPORTANT!!! We are working on an Arduino DUE, 
// so int is 32 bit (-2,147,483,648 to 2,147,483,647)
// For Arduino Uno int size is 8 bit, that is -32,768 to 32,767
// Use long or float if working with an Uno or simmilar 8-bit board
int PM25;
int PM10;

#define DHTPIN 5    // what digital pin we're connected to
// Choose two Arduino pins to use for software serial
#define DHTTYPE DHT22
TinyGPS gps;
SoftwareSerial gpsSerial(4, 3);
int sensorPin3 = A1; // select the input pin for the NH3
int sensorPin1 = A2; // select the input pin for the CO
int sensorPin2 = A5; // select the input pin for the NO2   
float flat, flon;
unsigned long age;
char msg[250];


DHT dht(DHTPIN, DHTTYPE);

void setup() {
Serial.begin(9600);
gpsSerial.begin(9600);
  while (!Serial);
  Serial.begin(9600);
  delay(100);
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
  pinMode(A0,INPUT);
  pinMode(A3,INPUT); 
  pinMode(A4,INPUT);

   Serial.println("initialization") ;
   Serial.println("DHTxx test!");

  dht.begin();

  Serial.println("LoRa Sender");

  if (!LoRa.begin(868.1E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
    // Stop autosend
  my_status = stop_autosend(); 
  // Serial print is used just for debugging
  // But one can design a more complex code if desired
  Serial.print("Stop autosend status is ");
  Serial.println(my_status, BIN);
  Serial.println(" ");
  delay(500);
  
  // Start fan
  my_status = start_measurement(); 
  // Serial print is used just for debugging
  // But one can design a more complex code if desired
  Serial.print("Start measurement status is ");
  Serial.println(my_status, BIN);
  Serial.println(" ");
  delay(5000);  

}
  

void loop() {
  // Wait a few seconds between measurements.
  my_status = read_measurement(); 
  // Serial print is used just for debugging
  // But one can design a more complex code if desired
  Serial.print("Read measurement status is ");
  Serial.println(my_status, BIN);
  Serial.print("PM2.5 value is ");
  Serial.println(PM25, DEC);
  Serial.print("PM10 value is ");
  Serial.println(PM10, DEC);
  Serial.println(" ");
  delay(6000);
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
 
  float mq135=analogRead(A0);
  float mq4=analogRead(A3);
  float mq3=analogRead(A4);
  float sensorValue1 = analogRead(sensorPin1);
  float sensorValue2 = analogRead(sensorPin2);
  float sensorValue3 = analogRead(sensorPin3);
  float no2 = 0.05+ sensorValue2*((10-0.05)/1023.00);
  float co = 1.00 + sensorValue1*((1000.0-1.00)/1023.00);
  float nh3 = 1.00 + sensorValue3*((500.0-1.0)/1023.00);
  float vrl135 = (mq135*5)/1023;
  float vrl4 = (mq4*5)/1023;
  float vrl3 = (mq3*5)/1023;  
  float rs135 = ((5-vrl135)*RL135)/vrl135;
  float rs4 = ((5-vrl4)*RL)/vrl4;
  float rs3 = ((5-vrl3)*RL3)/vrl3;
  float ratio135 = rs135/ro135; 
  float ratio4 = rs4/ro4;
  float ratio3 = rs3/ro3;  
  float co2 = 110.47*(pow(ratio135,-2.862));
  float alcohol = 0.3934*(pow(ratio3,-1.504));
  float ch4 = 1012.7*(pow(ratio4,-2.786));    
 bool newData = false;

 for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (gpsSerial.available())
    {
      char c = gpsSerial.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }
  if(newData)
  {
  gps.f_get_position(&flat, &flon, &age);
  }
  StaticJsonDocument<400> doc;
  doc["HUM"] =h;
  doc["TEMP"] =t;
  doc["HIC"] =hic;
  doc["LAT"] =(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat);
  doc["LON"] =(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon);
  doc["CO"] =co;
  doc["NO2"] =no2;
  doc["NH3"] =nh3;
  doc["CO2"] =co2;
  doc["ALC"] =alcohol;
  doc["CH4"] =ch4;
  doc["AQI"] =mq135;
  doc["pm25"]=PM25;
  doc["pm10"]=PM10;

    serializeJsonPretty(doc,msg);
    Serial.println(msg); 

    Serial.println("Sending to LoRa");
    LoRa.beginPacket();
    LoRa.print(msg);
    LoRa.endPacket();
    Serial.println("Sent via LoRa");
}
bool start_measurement(void)
{
  // First, we send the command
  byte start_measurement[] = {0x68, 0x01, 0x01, 0x96 };
  Serial.write(start_measurement, sizeof(start_measurement));
  //Then we wait for the response
  while(Serial.available() < 2);
  char read1 = Serial.read();
  char read2 = Serial.read();
  // Test the response
  if ((read1 == 0xA5) && (read2 == 0xA5)){
    // ACK
    return 1;
  }
  else if ((read1 == 0x96) && (read2 == 0x96))
  {
    // NACK
    return 0;
  }
  else return 0;
}

bool stop_measurement(void)
{
  // First, we send the command
  byte stop_measurement[] = {0x68, 0x01, 0x02, 0x95 };
  Serial.write(stop_measurement, sizeof(stop_measurement));
  //Then we wait for the response
  while(Serial.available() < 2);
  char read1 = Serial.read();
  char read2 = Serial.read();
  // Test the response
  if ((read1 == 0xA5) && (read2 == 0xA5)){
    // ACK
    return 1;
  }
  else if ((read1 == 0x96) && (read2 == 0x96))
  {
    // NACK
    return 0;
  }
  else return 0;
}

bool read_measurement (void)
{
  // Send the command 0x68 0x01 0x04 0x93
  byte read_particle[] = {0x68, 0x01, 0x04, 0x93 };
  Serial.write(read_particle, sizeof(read_particle));
  // A measurement can return 0X9696 for NACK
  // Or can return eight bytes if successful
  // We wait for the first two bytes
  while(Serial.available() < 1);
  byte HEAD = Serial.read();
  while(Serial.available() < 1);
  byte LEN = Serial.read();
  // Test the response
  if ((HEAD == 0x96) && (LEN == 0x96)){
    // NACK
    Serial.println("NACK");
    return 0;
  }
  else if ((HEAD == 0x40) && (LEN == 0x05))
  {
    // The measuremet is valid, read the rest of the data 
    // wait for the next byte
    while(Serial.available() < 1);
    byte COMD = Serial.read();
    while(Serial.available() < 1);
    byte DF1 = Serial.read(); 
    while(Serial.available() < 1);
    byte DF2 = Serial.read();     
    while(Serial.available() < 1);
    byte DF3 = Serial.read();   
    while(Serial.available() < 1);
    byte DF4 = Serial.read();     
    while(Serial.available() < 1);
    byte CS = Serial.read();      
    // Now we shall verify the checksum
    if (((0x10000 - HEAD - LEN - COMD - DF1 - DF2 - DF3 - DF4) % 0XFF) != CS){
      Serial.println("Checksum fail");
      return 0;
    }
    else
    {
      // Checksum OK, we compute PM2.5 and PM10 values
      PM25 = DF1 * 256 + DF2;
      PM10 = DF3 * 256 + DF4;
      return 1;
    }
  }
}

bool stop_autosend(void)
{
 // Stop auto send
  byte stop_autosend[] = {0x68, 0x01, 0x20, 0x77 };
  Serial.write(stop_autosend, sizeof(stop_autosend));
  //Then we wait for the response
  while(Serial.available() < 2);
  char read1 = Serial.read();
  char read2 = Serial.read();
  // Test the response
  if ((read1 == 0xA5) && (read2 == 0xA5)){
    // ACK
    return 1;
  }
  else if ((read1 == 0x96) && (read2 == 0x96))
  {
    // NACK
    return 0;
  }
  else return 0;
}

bool start_autosend(void)
{
 // Start auto send
  byte start_autosend[] = {0x68, 0x01, 0x40, 0x57 };
  Serial.write(start_autosend, sizeof(start_autosend));
  //Then we wait for the response
  while(Serial.available() < 2);
  char read1 = Serial.read();
  char read2 = Serial.read();
  // Test the response
  if ((read1 == 0xA5) && (read2 == 0xA5)){
    // ACK
    return 1;
  }
  else if ((read1 == 0x96) && (read2 == 0x96))
  {
    // NACK
    return 0;
  }
  else return 0;
}
