
#define F_CPU 8000000                          // clock speed: 8MHz (internal clock)
#include "TinyWireS.h"                          // I2C library for ATtiny84A (and other older ATtinys)

const byte SLAVE_ADDR = 0xa;

volatile byte position = 0;

void setup() {
    TinyWireS.begin(SLAVE_ADDR);
    TinyWireS.onReceive(requestISR);
}

void loop() {
  // This needs to be here
    TinyWireS_stop_check();
}

void requestISR(uint8_t mybyte) {
    
    position = mybyte;
    TinyWireS.write(position);

}
