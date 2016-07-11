//  Author:Frankie.Chu
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#include "TM1637.h"
#define CLK 2//pins definitions for TM1637 and can be changed to other ports       
#define DIO 3
TM1637 tm1637(CLK,DIO);

#define trigPin 5    //Trig - green Jumper
#define echoPin 6    //Echo - yellow Jumper


void setup()
{
//  tm1637.init();
//  tm1637.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void kloop()
{
  int8_t NumTab[] = {0,1,2,3,4,5,6,7,8,9};//,10,11,12,13,14,15};//0~9,A,b,C,d,E,F
  int8_t ListDisp[4];
  unsigned char i = 0;
  unsigned char count = 0;
  delay(150);
  while(1)
  {
    i = count;
    count ++;
    if(count == sizeof(NumTab)) count = 0;
    for(unsigned char BitSelect = 0;BitSelect < 4;BitSelect ++)
    {
      ListDisp[BitSelect] = NumTab[i];
      i ++;
      if(i == sizeof(NumTab)) i = 0;
    }
    tm1637.display(0,ListDisp[0]);
    tm1637.display(1,ListDisp[1]); 
    tm1637.display(2,ListDisp[2]);
    tm1637.display(3,ListDisp[3]);
    delay(300);
  }
}

#define MAX_DURATION 10000   // Max distance of about 3.4m

long getCM()
{
  long duration, cm;

  do
  {
    // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
   
    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH, MAX_DURATION);
 } while (duration >= MAX_DURATION || duration <= 0);
 
 
  // convert the time into a distance
  cm = (duration/2) / 29.1;

  Serial.print("Duration=");
  Serial.print(duration);
  Serial.print(" Distance=");
  Serial.println(cm);

  return cm; 
}

void Display(long n)
{
  tm1637.display(3,n%10);
  n /= 10;
  tm1637.display(2,n%10);
  n /= 10;
  tm1637.display(1,n%10);
  n /= 10;
  tm1637.display(0,n%10);
}


void loop()
{
  long cm[3];
  long min;
  long max;
  
  min = max = cm[0] = getCM();
  cm[1] = getCM();
  if (min < cm[1])
  {
    max = cm[1];
  }
  else
  {
    min = cm[1];
  }
  
  cm[2] = getCM();
  if (min > cm[2])
  {
    min = cm[2];
  }
  else if (max < cm[2])
  {
    max = cm[2];
  }
  
  long distance = cm[0] + cm[1] + cm[2] - min - max;

  Serial.print(distance);
  Serial.print("cm");
  Serial.print("  from ");
  Serial.print(cm[0]);
  Serial.print(' ');
  Serial.print(cm[1]);
  Serial.print(' ');
  Serial.print(cm[2]);
  Serial.print(" ; min=");
  Serial.print(min);
  Serial.print(" max=");
  Serial.print(max);
  Serial.println();

  
//  Display(distance);
  
  delay(250);
}
