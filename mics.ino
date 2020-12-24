int sensorPin3 = A1; // select the input pin for the NH3
int sensorPin1 = A2; // select the input pin for the CO
int sensorPin2 = A3; // select the input pin for the NO2


void setup() {

pinMode(sensorPin1, INPUT);
pinMode(sensorPin2, INPUT);
pinMode(sensorPin3, INPUT);
Serial.begin(9600);
Serial.println("initialization") ;

}

void loop() {
// read the value from the sensor:

float sensorValue1 = analogRead(sensorPin1);
float sensorValue2 = analogRead(sensorPin2);
float sensorValue3 = analogRead(sensorPin3);
float no2 = 0.05+ sensorValue2*((10-0.05)/1023.00);
float co = 1.00 + sensorValue1*((1000.0-1.00)/1023.00);
float nh3 = 1.00 + sensorValue3*((500.0-1.0)/1023.00);

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
delay(3000);

}
