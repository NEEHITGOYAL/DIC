#define RL 20
#define ro135 89.41
#define ro8 25.07
#define ro6 45.16
#define ro4 32.19
#define ro3 70.63
#define ro2 63.51
bool my_status;
 
// IMPORTANT!!! We are working on an Arduino DUE, 
// so int is 32 bit (-2,147,483,648 to 2,147,483,647)
// For Arduino Uno int size is 8 bit, that is -32,768 to 32,767
// Use long or float if working with an Uno or simmilar 8-bit board
float PM25;
float PM10;
 
void setup() {
pinMode(A0,INPUT);
pinMode(A1,INPUT);
pinMode(A2,INPUT);
pinMode(A3,INPUT);
pinMode(A4,INPUT);
pinMode(A5,INPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.begin(9600);
  delay(100);
  
 
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
/*  Serial.print("rs135 ");
  Serial.println(rs135);
  Serial.print("rs8 ");
  Serial.println(rs8);
  Serial.print("rs6 ");
  Serial.println(rs6);
  Serial.print("rs4 ");
  Serial.println(rs4);
  Serial.print("rs3 ");
  Serial.println(rs3);
  Serial.print("rs2 ");
  Serial.println(rs2);
  */
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
  float nh3 = 102.2*(pow(ratio135,-2.473));
  
  Serial.print("CO2 ");
  Serial.println(co2);
  Serial.print("Propane ");
  Serial.println(propane);
  Serial.print("Alcohol ");
  Serial.println(alcohol);
  Serial.print("CH4 ");
  Serial.println(ch4);
  Serial.print("LPG ");
  Serial.println(lpg);
  Serial.print("H2 ");
  Serial.println(h2);
  Serial.print("nh3 ");
  Serial.println(nh3);  
//  delay(2000);
  // Read the particle data every minute
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
  // Wait one minute
  delay(6000);
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
