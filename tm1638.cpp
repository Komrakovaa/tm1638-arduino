#include "tm1638.h"

Tm1638::Tm1638(uint8_t _stbPin, uint8_t _clkPin, uint8_t _dataPin)
    :stbPin(_stbPin), clkPin(_clkPin), dataPin(_dataPin)
{
    pinMode(stbPin, OUTPUT);
    pinMode(clkPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    setBrightness( TM1638_BRIGHTNESS_DEFAULT );

#ifdef TM1638_FAST_SHIFTOUT && __AVR__
  clkPort  = portOutputRegister(digitalPinToPort(clkPin));
  clkBitMask = digitalPinToBitMask(clkPin);
  
  dataPort = portOutputRegister(digitalPinToPort(dataPin));
  dataBitMask = digitalPinToBitMask(dataPin);
#endif    
}

Tm1638::~Tm1638()
{  
}

void Tm1638::setBrightness(uint8_t value){
    if( value == 0) {
      sendCommand( TM1638_BRIGHTNESS_OFF );
    } else {
      sendCommand( TM1638_BRIGHTNESS_BASE + (value-1) );
    }  
}

void Tm1638::setKeyPressCallback(void (*f)(uint8_t))
{
  button_down = f;
}


void Tm1638::setKeyReleaseCallback(void (*f)(uint8_t))
{
  button_up = f;
}


void Tm1638::sendCommand(uint8_t value)
{
	digitalWrite(stbPin, LOW);
	shiftOut(dataPin, clkPin, LSBFIRST, value);
	digitalWrite(stbPin, HIGH);
}

void Tm1638::setLED(uint8_t led, uint8_t value){
	tm1638_reg_data[(led - 1) * 2 + 1] = value;
}

void Tm1638::setChar(uint8_t pos, char c){
	tm1638_reg_data[(pos - 1) * 2] = FONT_DEFAULT[c - 32];
}

void Tm1638::displayNum(uint32_t num){
	char buf[8];
	sprintf(buf, "%8li", num);
	for (uint8_t i = 0; i<8; i++)
	{
		setChar(i + 1, buf[i]);
	}
}

void Tm1638::displayStr(const char* str)
{
	for (uint8_t i = 0; i < strlen(str); i++){
		setChar(i + 1, str[i]);
	}
}

uint8_t Tm1638::receive()
{
	uint8_t temp = 0;

	// Pull-up on
	pinMode(dataPin, INPUT);
	digitalWrite(dataPin, HIGH);

	for (uint8_t i = 0; i < 8; i++) {
		temp >>= 1;

		digitalWrite(clkPin, LOW);

		if (digitalRead(dataPin)) {
			temp |= 0x80;
		}

		digitalWrite(clkPin, HIGH);
	}

	// Pull-up off
	pinMode(dataPin, OUTPUT);
	digitalWrite(dataPin, LOW);

	return temp;
}

void Tm1638::display() {
	sendCommand(0x40);
	digitalWrite(stbPin, LOW);
	shiftOut(dataPin, clkPin, LSBFIRST, 0xC0);
	for (uint8_t i = 0; i < 16; i++)
	{
#ifdef TM1638_FAST_SHIFTOUT && __AVR__
		fastShiftOut(dataPin, clkPin,tm1638_reg_data[i]);
#else
    ShiftOut(dataPin, clkPin, LSBFIRST, tm1638_reg_data[i]);
#endif    
	}
	digitalWrite(stbPin, HIGH);
}

void Tm1638::readKeys() {		
	tm1638_keys = 0;
	digitalWrite(stbPin, LOW);
	shiftOut(dataPin, clkPin, LSBFIRST, 0x42);
	for (uint8_t i = 0; i < 4; i++) {
		tm1638_keys |= receive() << i;
	}
	digitalWrite(stbPin, HIGH);
	if (tm1638_keys != tm1638_keys_prev) {
		processEvents();
		tm1638_keys_prev = tm1638_keys;
	}
}

void Tm1638::processEvents(){
	uint8_t pressed = 0;
  uint8_t released = 0;  
	
	for (uint8_t i = 0; i < 8; i++){
		if (bitRead(tm1638_keys, i) && !bitRead(tm1638_keys_prev, i)) {
        bitSet(pressed,i);
		}

    if (!bitRead(tm1638_keys, i) && bitRead(tm1638_keys_prev, i)) {
        bitSet(released,i);
    }   
	}
  if ((pressed !=0) && (button_down!=0)) (*button_down)(pressed);
  if ((button_up !=0) && (button_up!=0)) (*button_up) (released);
}

#ifdef TM1638_FAST_SHIFTOUT && __AVR__
void Tm1638::fastShiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t val)
{
    uint8_t i;
    uint8_t tmp;

    for (i = 0; i < 8; i++)  {
         tmp = !!(val & (1 << i));
         if ( tmp ){
            *dataPort |= dataBitMask;
         } else  {
            *dataPort &=~ dataBitMask;                              
         }
        *clkPort |= clkBitMask;
        *clkPort &=~ clkBitMask;
    }
}
#endif
