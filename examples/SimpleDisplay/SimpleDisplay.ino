#include "tm1638.h"

#define STB 8
#define CLK 10
#define DATA 9

Tm1638 tm(STB,CLK,DATA);

bool blink = false;
uint32_t counter = 0;

void setup() {
  tm.setBrightness(8);
}

void loop() {
  tm.clearAll();
  tm.displayInt(counter);
  counter++;	
   
  if (	blink )
  {
    for (uint8_t i=1; i<=8;i++) 
		tm.setLED(i,1);    
  }
  
  blink = !blink;
  tm.display();
  delay(100);
}