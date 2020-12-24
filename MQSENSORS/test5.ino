#define _RLValue 22
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
 float mq135=analogRead(A5);
  float V_RL = mq135*5/1023;
  
  float _RS_Calc = (((5/V_RL)-1)*_RLValue);
  float x = _RS_Calc;
  for(int i =0;i<500;i++){
    _RS_Calc= _RS_Calc+x;
  };
  _RS_Calc=_RS_Calc/500;
  Serial.println("Ro");
  Serial.println(_RS_Calc);
  int y=map(_RS_Calc,30,200,10,300);
  Serial.println("ppm nh3");
  Serial.println(y);
  delay(500);
}
