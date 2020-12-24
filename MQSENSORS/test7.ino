#define mq2rl 10
#define mq3rl 200
#define mq4rl 20
#define mq6rl 22
#define mq8rl 10
#define mq135rl 22
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
float rscalc(int mq,int _RLValue)
{
  
  float V_RL = mq*5/1023;
  
  float _RS_Calc = (((5/V_RL)-1)*_RLValue);
  float x = _RS_Calc;
  for(int i =0;i<500;i++){
    _RS_Calc= _RS_Calc+x;
  };
  _RS_Calc=_RS_Calc/500;
  
  return _RS_Calc;
}
void loop() {
  // put your main code here, to run repeatedly:
  int mq2,mq3,mq4,mq6,mq8,mq135;
  mq2 = analogRead(A1);
  mq3 = analogRead(A4);
  mq4 = analogRead(A5);
  mq6 = analogRead(A2);
  mq8 = analogRead(A3);
  mq135 = analogRead(A0);
/*  
  
*/
float mq2rs = rscalc(mq2,mq2rl);
float mq3rs = rscalc(mq3,mq3rl);
float mq4rs = rscalc(mq4,mq4rl);
float mq6rs = rscalc(mq6,mq6rl);
float mq8rs = rscalc(mq8,mq8rl);
float mq135rs = rscalc(mq135,mq135rl);
int ammonia=map(mq135rs,30,200,300,10);
int propane=map(mq2rs,2,20,10000,100);
int alcohol=map(mq3rs,1000,8000,10,0.05);
int methane=map(mq4rs,10,60,10000,200);
int lpg=map(mq6rs,10,60,10000,200);
int h2=map(mq8rs,10,60,10000,100);
/*Serial.print("mq2rs:");
  Serial.println(mq2rs);
  Serial.print("mq3rs:");
  Serial.println(mq3rs);
  Serial.print("mq4rs:");
  Serial.println(mq4rs);
  Serial.print("mq6rs:");
  Serial.println(mq6rs);
  Serial.print("mq8rs:");
  Serial.println(mq8rs);
  Serial.print("mq135rs:");
  Serial.println(mq135rs);    
  delay(3000);  
*/  Serial.print("mq2:");
  Serial.println(mq2);
  Serial.print("mq3:");
  Serial.println(mq3);
  Serial.print("mq4:");
  Serial.println(mq4);
  Serial.print("mq6:");
  Serial.println(mq6);
  Serial.print("mq8:");
  Serial.println(mq8);
  Serial.print("mq135:");
  Serial.println(mq135);    
  delay(3000);  
/*  Serial.print("Ammonia ");
  Serial.println(ammonia);
  Serial.print("Propane ");
  Serial.println(propane);
   Serial.print("Alcohol ");
  Serial.println(alcohol);
  Serial.print("Methane ");
  Serial.println(methane);
   Serial.print("LPG ");
  Serial.println(lpg);
  Serial.print("H2 ");
  Serial.println(h2);
  delay(3000);
*/
}
