//Code for the Arduino Mega<
#include <Wire.h>

const byte addr = 0xa;
byte current = 0;
 
void setup()
{
 Wire.begin(0xa); // join i2c bus (address optional for master)
 Wire.onRequest(requestEvent);
 Serial.begin(9600); // start serial for output
}
 
void loop()
{
 
}

void requestEvent(){
  Serial.println("Request Received");
  if (current==0){
    Wire.write(1);
    current=1;
    Wire.endTransmission();
  }else{
    Wire.write(0);
    current=0;
    Wire.endTransmission();
  }
}
