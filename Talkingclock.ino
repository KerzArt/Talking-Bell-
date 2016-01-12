/*******************************************************************************
 * DFPlayer_Mini_Mp3, This library provides a quite complete function for      * 
 * DFPlayer mini mp3 module.                                                   *
 * www.github.com/dfrobot/DFPlayer_Mini_Mp3 (github as default source provider)*
 *  DFRobot-A great source for opensource hardware and robot.                  *
 *                                                                             *
 * This file is part of the DFplayer_Mini_Mp3 library.                         *
 *                                                                             *
 * DFPlayer_Mini_Mp3 is free software: you can redistribute it and/or          *
 * modify it under the terms of the GNU Lesser General Public License as       *
 * published by the Free Software Foundation, either version 3 of              *
 * the License, or any later version.                                          *
 *                                                                             *
 * DFPlayer_Mini_Mp3 is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 *                                                                             *
 * DFPlayer_Mini_Mp3 is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public            *
 * License along with DFPlayer_Mini_Mp3. If not, see                           *
 * <http://www.gnu.org/licenses/>.                                             *
 *                         *
 ******************************************************************************/

/*
 *  Copyright:  DFRobot
 *  name:   DFPlayer_Mini_Mp3 sample code
 *  Author:   lisper <lisper.li@dfrobot.com>
 *  Date:   2014-05-30
 *  Description:  connect DFPlayer Mini by SoftwareSerial, this code is test on Uno
 *      Note: the mp3 files must put into mp3 folder in your tf card 
 */
//#include "leOS2.h" //include the scheduler
//leOS2 myOS; //create a new istance
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

SoftwareSerial mySerial(3, 2); // RX, TX

unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change :
const long interval = 500;           // interval at which to blink (milliseconds)

const int BTN=10;
const int BUSY=4;
const int LIFE=11;
const int BUSYLED=12;
const int FULLPIN=14;
const int CHARGEPIN=15;

#define BATTPIN A2


int tekan=0;
int charge=0;
int full=0;
int low=0;
int c=0;
int s1=0;
int s2=0;
int still=0;
float voltage=0;
float tempvoltage=0;
byte LEDstate = 1;


//
void setup () {
  analogReference(EXTERNAL);
  pinMode(BTN,INPUT);
  pinMode(BUSY,INPUT_PULLUP);
  pinMode(FULLPIN,INPUT_PULLUP);
  pinMode(CHARGEPIN,INPUT_PULLUP);
  pinMode(BUSYLED,OUTPUT);
  pinMode(LIFE,OUTPUT);
  Serial.begin (9600);
  mySerial.begin (9600);
  mp3_set_serial (mySerial);  //set softwareSerial for DFPlayer-mini mp3 module 
  mp3_set_volume (20);delay (10);
  mp3_play (79);delay (2000);
  mp3_stop();delay (10);
  mp3_set_volume (15);  
}



//
void loop () {    
  scanbtn();
  voltage = 0;
  c = 0;
  
  while(tekan==0){
    unsigned long currentMillis = millis();
    scanbtn();
    if(tekan==1){break;}
    c++;
    tempvoltage = analogRead(BATTPIN) * (4.2 / 1023.0);  
    voltage = voltage + tempvoltage;  
    if(c>10){
      voltage = voltage / 10;
      c = 0;
      //Serial.print(charge);
      //Serial.print(" ");
      //Serial.print(full);
      //Serial.print(" ");
      //Serial.println(voltage);
    }
 
    if(digitalRead(CHARGEPIN)==LOW){charge=1;mp3_set_volume (15);delay (10);}else{charge=0;mp3_set_volume (30);delay (10);}
    if(digitalRead(FULLPIN)==LOW){full=1;mp3_set_volume (30);delay (10);}else{full=0;}
    if(voltage<=1.85){low=1;mp3_set_volume (30);delay (10);}else{low=0;}

    if(low==1){
      if(s1==0){
        s1=1;
        mp3_play (66);
        delay (6000);
        mp3_stop();delay (10);  
      }
    }else{
      if(s1==1){
        s1=0; 
      }  
    }

    if(full==1){
      if(s2==0){
        s2=1;
        mp3_play (67);
        delay (4000);
        mp3_stop();delay (10);  
      }
    }else{
      if(s2==1){
        s2=0; 
      }  
    }
    
    if(charge==1){
      if(still==0){
        still=1;
        mp3_play (81);
        delay (1000);
        mp3_stop();delay (10);  
      }
    }else{
      if(still==1){
        still=0;
        mp3_set_volume (15);delay (10);
        mp3_play (82);
        delay (1000);
        mp3_stop();delay (10);  
      }  
    }

    mp3_set_volume (30);
    
    if(currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED 
      previousMillis = currentMillis;   
      flashLed();     
    }
    
    if(tekan==1){
      digitalWrite(BUSYLED,HIGH);delay(10);
    }else{
      digitalWrite(BUSYLED,LOW);delay(10);
    }
  }
  
  while(tekan==1){
    mp3_play (100);delay(1000);
    while(digitalRead(BUSY)==LOW){};
    mp3_stop();
    tekan=0;
  }
}

void flashLed() {
    LEDstate ^= 1;
    digitalWrite(LIFE, LEDstate);
}


//this task will flash the 2nd LED
void scanbtn() {
  if(digitalRead(BTN)==HIGH){ 
    while(digitalRead(BTN)==HIGH){}
    tekan=1;
  }   
}

