# tm1638-arduino
TM1638 Board library for Arduino

# Introduction
This library will help you use the TM1638 based LED & Key board.
The main feature of this library,  it does not work with single leds and buttons.
For segment displays and leds you set what you need, call `transfer()`,  and your "image" transfered to the TM1638. Typical usage is to call `transfer()`  from timer event.
The same is for keys, but you can set your callback for buttons press and release.

# Usage

#### Install
1) Get all files, create tm1638-arduino.zip( zip from GitHub will not work);  
2) Arduino IDE -> add Zip library;  
3) Use examples from File->Examples for fast start.  


#### Constructor
`Tm1638  tm1638  ( stbPin,  clkPin, dataPin); `  
where  **stbPin,  clkPin, dataPin** pins you connected to the corresponding pins of the TM1638 board.  

#### Setup
Set leds **brightness** 0..8,   0 - is off.  
`tm1638.setBrightness( brightness );`  

Setup callbacks. Callback receive  **uint8_t,** where one bit is status of one key.  
`tm1638.setKeyPressCallback( onKeyPress );`  
`tm1638.setKeyReleaseCallback( onKeyRelease );`  

#### Display
Use this methods to prepare what you want to display.  

`tm1638.setLED( led, t value );`  
`tm1638.setChar( pos, char );`  
`tm1638.displayNum( number );`  
`tm1638.displayStr( string );`  

Call **transfer() ** to display.  
`tm1638.transfer();`  

