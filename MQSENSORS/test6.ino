#define RL 20
#define ro135 76.63
#define ro8 1
#define ro6 13.4285714
#define ro4 47.775076 
#define ro3 3.86018237
#define ro2 9.659574468

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
pinMode(A0,INPUT);
pinMode(A1,INPUT);
pinMode(A2,INPUT);
pinMode(A3,INPUT);
pinMode(A4,INPUT);
pinMode(A5,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  float mq135=analogRead(A0);
  float mq8=analogRead(A1);
  float mq6=analogRead(A2);
  float mq4=analogRead(A3);
  float mq3=analogRead(A4);
  float mq2=analogRead(A5);
  float vrl135 = (mq135*5)/1023;
  float vrl8 = (mq8*5)/1023;
  float vrl6 = (mq6*5)/1023;
  float vrl4 = (mq4*5)/1023;
  float vrl3 = (mq3*5)/1023;
  float vrl2 = (mq2*5)/1023;
  float rs135 = ((5-vrl135)*RL)/vrl135;
  float rs8 = ((5-vrl8)*RL)/vrl8;
  float rs6 = ((5-vrl6)*RL)/vrl6;
  float rs4 = ((5-vrl4)*RL)/vrl4;
  float rs3 = ((5-vrl3)*RL)/vrl3;
  float rs2 = ((5-vrl2)*RL)/vrl2;
  float ratio135 = rs135/ro135;
  float ratio8 = rs8/ro8;
  float ratio6 = rs6/ro6;
  float ratio4 = rs4/ro4;
  float ratio3 = rs3/ro3;
  float ratio2 = rs2/ro2;
  float co2 = 110.47*(pow(ratio135,-2.862));
  float propane = 652.71*(pow(ratio2,-2.168));
  float alcohol = 0.3934*(pow(ratio3,-1.504));
  float ch4 = 1012.7*(pow(ratio4,-2.786));
  float lpg = 1009.2*(pow(ratio6,-2.35));
  float h2 = 976.97*(pow(ratio8,-0.688));
  
  Serial.print("CO2 ");
  Serial.println(co2);
  Serial.print("CPropane ");
  Serial.println(propane);
  Serial.print("Alcohol ");
  Serial.println(alcohol);
  Serial.print("CH4 ");
  Serial.println(ch4);
  Serial.print("LPG ");
  Serial.println(lpg);
  Serial.print("H2 ");
  Serial.println(h2);
  delay(2000);
 
}
