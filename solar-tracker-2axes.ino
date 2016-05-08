// http://www.electronicshub.org/arduino-solar-tracker/
//#include <Sleep_n0m1.h> // from here https://github.com/n0m1/Sleep_n0m1
#include <JeeLib.h> // Low power functions library https://github.com/jcw/jeelib
#include <Servo.h>
//defining Servos

//#include <LowPower.h>
#include <Vcc.h> // from here https://github.com/Yveaux/arduino_vcc
#define DEBUG  // if uncomment debug will print on serial

const float VccMin   = 0.0;           // Minimum expected Vcc level, in Volts.
const float VccMax   = 4.2;           // Maximum expected Vcc level, in Volts.
const float VccCorrection = 3.98/4.137;  // Measured Vcc by multimeter divided by reported Vcc

Vcc vcc(VccCorrection);

ISR(WDT_vect) { Sleepy::watchdogEvent(); } // Setup the watchdog
//Sleep sleep;
//unsigned long sleepTime; //how long you want the arduino to sleep
   
Servo servohori;
int servoh = 0;
int servohLimitHigh = 160;
int servohLimitLow = 20;

Servo servoverti; 
int servov = 0; 
int servovLimitHigh = 160;
int servovLimitLow = 20;

//Assigning LDRs
int ldrtopl = 2; //top left LDR green
int ldrtopr = 1; //top right LDR yellow
int ldrbotl = 3; // bottom left LDR blue
int ldrbotr = 0; // bottom right LDR orange
int volt=6;          //variable to store the values form the analog pin 2  

 void setup () 
 {
  Serial.begin(115200);
  analogReference(DEFAULT);
  servohori.attach(11);
  servohori.write(0);
  servoverti.attach(8);
  servoverti.write(0);
  delay(500);
//  sleepTime = 50000; //set sleep time in ms, max sleep time is 49.7 days
 }

void loop()
{
  servoh = servohori.read();
  servov = servoverti.read();
  
  //capturing analog values of each LDR
  int topl = analogRead(ldrtopl);
  int topr = analogRead(ldrtopr);
  int botl = analogRead(ldrbotl);
  int botr = analogRead(ldrbotr);
  float v = vcc.Read_Volts();
  
  #ifdef DEBUG
      Serial.print("Top Left: ");
      Serial.println(topl);
      Serial.print("Top right: ");
      Serial.println(topr);
      Serial.print("Bottom Left: ");
      Serial.println(botl);
      Serial.print("Bottom Right: ");
      Serial.println(botr);
      Serial.print("Voltage: ");
      Serial.print(v);
      Serial.println(" Volts");
  #endif
 
 // calculating average
  int avgtop = (topl + topr) / 2; //average of top LDRs
  int avgbot = (botl + botr) / 2; //average of bottom LDRs
  int avgleft = (topl + botl) / 2; //average of left LDRs
  int avgright = (topr + botr) / 2; //average of right LDRs
  
  #ifdef DEBUG
      Serial.print("Average top: ");
      Serial.println(avgtop);
      Serial.print("Average bottom: ");
      Serial.println(avgbot);
      Serial.print("Average Left: ");
      Serial.println(avgleft);
      Serial.print("Average Right: ");
      Serial.println(avgright);
  #endif
  
if (avgright == avgleft == avgbot == avgtop)
{
  #ifdef DEBUG
      Serial.print("Going to sleep ");
      Serial.print(10);
      Serial.println(" seconds");
      delay(100); //delay to allow serial to fully print before sleep
  #endif
  Sleepy::loseSomeTime(10000);
}
  else {
  Serial.println("debug no sleep"); 
  if (avgtop < avgbot)
   {
    servoverti.write(servov +1);
    if (servov > servovLimitHigh) 
     { 
      servov = servovLimitHigh;
     }
    delay(10);
  }
  else if (avgbot < avgtop)
  {
   servoverti.write(servov -1);
   if (servov < servovLimitLow)
      {
      servov = servovLimitLow;
      }
   delay(10);
  }
  else 
  {
   servoverti.write(servov);
  }
  
  if (avgleft > avgright)
  {
   servohori.write(servoh +1);
   if (servoh > servohLimitHigh)
    {
      servoh = servohLimitHigh;
    }
   delay(10);
  }
  else if (avgright > avgleft)
  {
    servohori.write(servoh -1);
    if (servoh < servohLimitLow)
     {
      servoh = servohLimitLow;
     }
    delay(10);
  }

  else 
  {
    servohori.write(servoh);
  }
  }
  delay(500);
}
