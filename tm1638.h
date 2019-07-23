#ifndef _TM1638_H_
#define _TM1638_H_

#define TM1638_FAST_SHIFTOUT

#include <arduino.h>
#include "tm16XXFonts.h"

#define TM1638_BRIGHTNESS_OFF  0x80
#define TM1638_BRIGHTNESS_BASE  0x88
//Possible values 0..8,   0 - Off
#define TM1638_BRIGHTNESS_DEFAULT 5

class Tm1638
{
public:
	uint8_t stbPin, clkPin, dataPin;
	volatile uint8_t tm1638_reg_data[16];
	volatile uint8_t tm1638_keys, tm1638_keys_prev;	
	
	Tm1638(uint8_t _stbPin, uint8_t _clkPin, uint8_t _dataPin);		
	~Tm1638();
  
  void display();
  void readKeys();
	
	void setBrightness( uint8_t value );
	void setLED(uint8_t led, uint8_t value);
	void setChar(uint8_t pos, char c);
	void displayNum(uint32_t num);
	void displayStr(const char* str);			
  void setKeyPressCallback(void (*f)(uint8_t));
  void setKeyReleaseCallback(void (*f)(uint8_t));

private:  
  void (*button_down)(uint8_t) = 0;
  void (*button_up)(uint8_t) = 0; 
  void sendCommand(uint8_t value);	
  void processEvents();
  uint8_t receive();

#ifdef TM1638_FAST_SHIFTOUT && __AVR__
  uint8_t *clkPort;
  uint8_t clkBitMask;
  
  uint8_t *dataPort;
  uint8_t dataBitMask;  
  void  fastShiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t val);
#endif 
  
};

#endif
