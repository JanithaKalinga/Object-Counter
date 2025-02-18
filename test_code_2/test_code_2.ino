#include <SPI.h>       
#include <DMD.h>    
#include <TimerOne.h>  
#include "Arial_black_16.h"
#include "Arial_Black_16_ISO_8859_1.h"
#include "Arial14.h"
#include "SystemFont5x7.h"

#define counter_pin 2

#define DISPLAYS_ACROSS 2 //-> Number of P10 panels used, side to side.
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

int count = 0;
bool pressed = false;

volatile float encoder_ticks = 0;
char formattedString[200];

char *Text = "";

void ScanDMD() { 
  dmd.scanDisplayBySPI();
  
}

void setup() {
    
  Serial.begin(115200);
  pinMode(counter_pin, INPUT_PULLUP);
  dmd.selectFont(Arial_Black_16_ISO_8859_1);
  attachInterrupt(digitalPinToInterrupt(2), CountA, CHANGE);
  delay(10);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(ScanDMD);
  dmd.clearScreen( true );

  Text = "Welcome to LC RM Department";

  dmd.drawMarquee(Text,strlen(Text),(32*DISPLAYS_ACROSS)-1,0);
  delay(100);
  //dmd.drawMarquee("COUNTER IS READY", 16, 64, 0);

  long start=millis();
  long timer=start;
  boolean ret=false;
  int interval=100;
  while(!ret){
    if ((timer+interval) < millis()) {
      ret=dmd.stepMarquee(-1,0);
      timer=millis();
      Serial.println(ret);
    }
  }
  dmd.clearScreen( true );
}

void loop() {
  CountA();
  delay(500);
}


void CountA() {
  bool currentstate = digitalRead(counter_pin);

  if (currentstate == true) {
    dmd.clearScreen( true );
    count++;
    sprintf(formattedString, "%04d", count);
    String Text1 = String(formattedString);
    Serial.println(formattedString);
    dmd.selectFont(Arial_Black_16);
    dmd.drawMarquee(formattedString, 4, 13, 1);
    dmd.stepMarquee(0, 0);
    while(digitalRead(counter_pin)==pressed){
      //
    }
  }
  
}
