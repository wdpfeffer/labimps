#include <Wire.h>                          // I2C library for ATtiny84A (and other older ATtinys)

#define F_CPU 8000000                          // clock speed: 8MHz (internal clock)
#define led 1



const byte SLAVE_ADDR = 0xa;

volatile byte position = 0;

void setup() {
    Wire.begin();
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
  
}

void loop() {

  while(Wire.available()){
    position = Wire.read();
  }

  if (position==0){
    digitalWrite(led, LOW);
  }else{
    digitalWrite(led, HIGH);
  }

  delay(500);
}
