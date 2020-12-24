
#define RL 20  //The value of resistor RL is 47K
//#define m -0.477 //Enter calculated Slope 
//#define b 1.064 //Enter calculated intercept
#define Ro 76.63 //Enter found Ro value
#define MQ_sensor A0 //Sensor is connected to A4
#define a 110.47
#define b -2.862



void setup() {
  Serial.begin(9600);
  
}

void loop() {

  float VRL; //Voltage drop across the MQ sensor
  float Rs; //Sensor resistance at gas concentration 
  float ratio; //Define variable for ratio
   
  VRL = analogRead(MQ_sensor)*(5.0/1023.0); //Measure the voltage drop and convert to 0-5V
  Rs = ((5.0*RL)/VRL)-RL; //Use formula to get Rs value
 // ratio = (Rs/Ro);  // find ratio Rs/Ro
float  ppm = a*(Rs/Ro)^b;
 // float ppm = pow(10, ((log10(ratio)-b)/m)); //use formula to calculate ppm

  
  Serial.print("CO  ");
  Serial.println(ppm);
 // Serial.print("Voltage  ");
 // Serial.println(VRL);
  

   delay(200);
  
}
