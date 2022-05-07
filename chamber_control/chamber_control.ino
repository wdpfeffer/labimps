#include <Adafruit_SSD1306_STM32.h>
#include <Wire.h>

#define lightLED PB3
#define swLight PA1
#define temp 0
#define hum 1
#define sensorInterval 10000 //read every 10
#define debounceDelay 50 //pushbutton interval

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


byte lightsButtonReading;
byte lightsButtonState;
byte lastLightsButtonState = HIGH;
byte lightsState = LOW;

unsigned long sensorCycleStart;
unsigned long sensorCycleCurrent;
unsigned long lastDebounceTimeLights;


void setup() {
  // put your setup code here, to run once:
  pinMode(swLight, INPUT_PULLDOWN);
  
  pinMode(lightLED, OUTPUT);
  pinMode(PC13, OUTPUT);   //board status
  sensorCycleStart = millis();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.write('Hello World');
 
}

void loop() {
  // put your main code here, to run repeatedly:

//  //Determine if we need a sensor read
//  if (millis()-sensorCycleStart > sensorInterval){
//    //Read temp and humidity
//
//    //update the display
//
//    //reset sensorCycleStart
//    sensorCycleStart = millis();
//    Serial.println("Sensor Read Triggered");
//    if (digitalRead(PC13)==HIGH){
//      digitalWrite(PC13, LOW);
//      //digitalWrite(lightLED, LOW);
//    }else{
//      digitalWrite(PC13, HIGH);
//      //digitalWrite(lightLED, HIGH);
//    }
//  }

  //Evaluate button pusheu

  lightsButtonReading = digitalRead(swLight);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (lightsButtonReading != lastLightsButtonState) {
    // reset the debouncing timer
    lastDebounceTimeLights = millis();
  }

  if ((millis() - lastDebounceTimeLights) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (lightsButtonReading != lightsButtonState) {
      lightsButtonState = lightsButtonReading;

      //toggle the LED
      if (lightsButtonState == HIGH){
        lightsState = !lightsState;
      }
    }
  }

  //Set the lights state
  digitalWrite(lightLED, lightsState);
  
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastLightsButtonState = lightsButtonReading;

}
