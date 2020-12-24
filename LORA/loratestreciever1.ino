#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) 
  {

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
      
    }

    // print RSSI of pack
    Serial.println("");
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
  }
