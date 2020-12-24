
 /*
 * This is the Arduino code Ublox NEO-6M GPS module
 * this code extracts the GPS latitude and longitude so it can be used for other purposes

 * 
 * Written by Ahmad Nejrabi for Robojax Video
 * Date: Jan. 24, 2017, in Ajax, Ontario, Canada
 * Permission granted to share this code given that this
 * note is kept with the code.
 * Disclaimer: this code is "AS IS" and for educational purpose only.
 * 
 */

// written for RoboJax.com 
#include<SPI.h>
#include<RH_RF95.h>
RH_RF95 rf95;
char data[100]={"hi"};
char data2[100];
String inputString = ""; // a string to hold incoming data
boolean stringComplete = false; // whether the string is complete
String signal = "$GPGLL";
void setup() {
    // initialize serial:
    inputString.reserve(200);
    Serial.begin(9600);
    while(!Serial)
    if(!rf95.init())
    Serial.print("init failed");
}

void loop() {
    // print the string when a newline arrives:
  
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
    if (stringComplete) {
        String BB = inputString.substring(0, 6);
        if (BB == signal) {
            String LAT = inputString.substring(7, 19);
            int LATperiod = LAT.indexOf('.');
            int LATzero = LAT.indexOf('0');
            if (LATzero == 0) {
                LAT = LAT.substring(1);
            }

            String LON = inputString.substring(20, 31);
            int LONperiod = LON.indexOf('.');
            int LONTzero = LON.indexOf('0');
            if (LONTzero == 0) {
                LON = LON.substring(1);
            }

            Serial.println(LAT);
            Serial.println(LON);
            LAT.toCharArray(data,sizeof(data));
            LON.toCharArray(data2,sizeof(data2));
            
            strcat(data,data2);
            Serial.print(data);
            //rf95.send(data, sizeof(data));

        }

        // Serial.println(inputString);
        // clear the string:
        inputString = "";
        stringComplete = false;
    }
}

/*
SerialEvent occurs whenever a new data comes in the
hardware serial RX. This routine is run between each
time loop() runs, so using delay inside loop can delay
response. Multiple bytes of data may be available.
*/
void serialEvent() {
    while (Serial.available()) {
        // get the new byte:
        char inChar = (char) Serial.read();
        // add it to the inputString:
        inputString += inChar;
        //rf95.send(data, sizeof(data));
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
        if (inChar == '\n')
        {
            stringComplete = true;
        }
    }
}
