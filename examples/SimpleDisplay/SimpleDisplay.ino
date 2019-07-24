#include "tm1638.h"

#define STB 8
#define CLK 10
#define DATA 9

Tm1638 tm(STB,CLK,DATA);

void setup() {
  tm.setBrightness(8);
}

void loop() {
  tm.setLED(1,1);
  tm.setLED(8,1);
  tm.displayNum(12345678);
  tm.display();
  
  delay(1000);
  
  tm.clearAll();
  tm.displayStr("Hello");
  tm.display();

  delay(1000);
}
