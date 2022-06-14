
#define F_CPU 8000000                          // clock speed: 8MHz (internal clock)
#include "TinyWireS.h"                          // I2C library for ATtiny84A (and other older ATtinys)

const byte SLAVE_ADDR = 0xa;
const byte availCmds[] = {0, 1, 2};

volatile byte position = 0;

void setup() {
    TinyWireS.begin(SLAVE_ADDR);
    TinyWireS.onReceive(requestISR);
}

void loop() {
  // This needs to be here
    TinyWireS_stop_check();
}

void requestISR(uint8_t howMany) {
    
    position = howMany;

    switch (position) {
      case 0{
        
      };
      case 1{
      
      }
    }

    TinyWireS.write(1);

}