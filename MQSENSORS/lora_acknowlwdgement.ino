#include <SPI.h>
#include <LoRa.h>
int counter = 0;
void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(868.1E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  char packet;
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
       packet = LoRa.read();
       Serial.print(packet);
       if(packet =='F')
       { break; 
       }
     // Serial.print((char)LoRa.read());
      
    }
     Serial.print("Sending packet: ");
  Serial.println(counter);

 while(1) { // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
  }

    // print RSSI of packet
    Serial.print(packet);
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
