#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include "DHT.h"
#define RL 20
#define RL135 5
#define RL3 200
#define ro135 52.86//89.41
#define ro4 14.08//32.19
#define ro3 1246.2//70.63

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

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
  pinMode(A0,INPUT);
  pinMode(A3,INPUT); 
  pinMode(A4,INPUT);
  Serial.begin(9600);
   gpsSerial.begin(9600);
   Serial.println("initialization") ;
   Serial.println("DHTxx test!");

  dht.begin();
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

//  LoRa.setTxPower(20);
  
}

void loop() {
  // Wait a few seconds between measurements.
  delay(3000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
/*  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
*/
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
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
  Serial.print("AQI: ");
  Serial.println(mq135);  
  Serial.print("CO2 ");
  Serial.println(co2);  
  Serial.print("Alcohol ");
  Serial.println(alcohol);
  Serial.print("CH4 ");
  Serial.println(ch4);   
  Serial.println("co sensor value = " + String(sensorValue1)) ;
  Serial.print("CO ppm = ") ;
  Serial.println(co);
  Serial.println("");
  Serial.println("no2 sensor value = " + String(sensorValue2)) ;
  Serial.print("NO2 ppm = ") ;
  Serial.println(no2);
  Serial.println("");
  Serial.println("nh3 sensor value = " + String(sensorValue3)) ;
  Serial.print("Nh3 ppm = ") ;
  Serial.println(nh3); 
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
//  if(newData)
  //{
    gps.f_get_position(&flat, &flon, &age);
    Serial.println("Sending to LoRa");
    LoRa.beginPacket();
    LoRa.print("Humidity: ");
    LoRa.print(h);
    LoRa.print(" %\t");
    LoRa.print("Temperature: ");
    LoRa.print(t);
    LoRa.print(" *C ");
    LoRa.print(f);
    LoRa.print(" *F\t");
    LoRa.print("Heat index: ");
    LoRa.print(hic);
    LoRa.print(" *C ");
    LoRa.print(hif);
    LoRa.println(" *F");
    LoRa.print("Lat: ");
    LoRa.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    LoRa.print(",");
    LoRa.print("Long: ");
    LoRa.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    LoRa.endPacket();
    delay(1000);
    LoRa.beginPacket();     
    LoRa.print("CO ppm = ") ;
    LoRa.println(co);
    LoRa.print("NO2 ppm = ") ;
    LoRa.println(no2);
    LoRa.print("Nh3 ppm = ") ;
    LoRa.println(nh3);
    LoRa.print("AQI: ");
    LoRa.println(mq135);
    LoRa.print("CO2 ");
    LoRa.println(co2);  
    LoRa.print("Alcohol ");
    LoRa.println(alcohol);
    LoRa.print("CH4 ");
    LoRa.println(ch4);    
    Serial.println("Sent via LoRa");
    LoRa.endPacket();
  //}

}
