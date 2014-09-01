//--------------------------------------------------------------
//-- A server for byte-sized commands, first 2 most significant
//-- bits reserved for direction, rest for PWM ms.
//--------------------------------------------------------------
//-- (c) Juan G Victores, Jun 2012
//-- (c) Juan Gonzalez-Gomez (Obijuan), Dec 2011
//-- GPL license
//--------------------------------------------------------------

#include <Servo.h> 

//-- Array for accesing the 2 servos
Servo myservo[2];  

int incomingByte = 0;   // for incoming serial data

void setup() 
{
  //-- Attaching the 2 servos
  myservo[0].attach(9);
  myservo[1].attach(10);
  
  Serial.begin(9600);
}  

void loop() { 
  
    //-- wait to receive data...
    if (Serial.available() > 0)
    {
        //-- Read the incoming byte:
        incomingByte = Serial.read();

        //-- Say what you got:
        //Serial.print("I received 0x");
        //Serial.println(incomingByte, HEX);
        
        int dir = incomingByte >> 6;
        //Serial.print("--> direction: ");
        //Serial.println(dir, DEC);

        int vel = (incomingByte & 0x3f) * 46;
        //Serial.print("--> velocity: ");
        //Serial.println(vel, DEC);

        myservo[dir].writeMicroseconds(vel);
        
    }       
} 

