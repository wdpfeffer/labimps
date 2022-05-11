#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


#define buttonLights 2  //input of the light button
#define buttonVent 3   //input of the vents button
#define buttonFan 4     //input for the fan button
#define LEDlights A0
#define LEDVent A1
#define LEDFan A2
#define temp 0
#define hum 1
#define sensorInterval 10000 //read every 10
#define debounceDelay 50 //pushbutton interval

#define DHTPIN 6  //digital pin for temp and humidity readings
#define DHTTYPE DHT22  //The type of dht probe we are using

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DHT dht(DHTPIN, DHTTYPE);

byte readingButtonLights;
byte stateButtonLights;
byte lastStateButtonLights = HIGH;
byte stateLights = LOW;

byte readingButtonFan;
byte stateButtonFan;
byte lastStateButtonFan = HIGH;
byte stateFan = LOW;

byte readingButtonVent;
byte stateButtonVent;
byte lastStateButtonVent = HIGH;
byte stateVent = LOW;

unsigned long sensorCycleStart;
unsigned long sensorCycleCurrent;
unsigned long lastDebounceTimeLights;
unsigned long lastDebounceTimeFan;
unsigned long lastDebounceTimeVent;



void setup() {
  // Start Serial
  Serial.begin(9600);

  //Setup pins
  pinMode(buttonLights, INPUT);
  pinMode(buttonVent, INPUT);
  pinMode(buttonFan, INPUT);

  pinMode(LEDlights, OUTPUT);
  pinMode(LEDFan, OUTPUT);
  pinMode(LEDVent, OUTPUT);

  pinMode(13, OUTPUT);   //board status
  
  //setup loop details
  sensorCycleStart = millis();

  //Setup display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.display();
  display.clearDisplay();
  delay(200);
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.write("Hello World");
  display.display();

  //setup dht
  dht.begin();
 
}

void loop() {
  // put your main code here, to run repeatedly:

 //Determine if we need a sensor read
  if (millis()-sensorCycleStart > sensorInterval){
    //Read temp and humidity
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    
    //Display temp and humidity
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    
    char temperature[5];
    dtostrf(t,5,1,temperature);
    
    display.write("Temp: ");
    display.write(temperature);
    display.display();

    char humidity[5];
    dtostrf(h,5,1,humidity);

    display.setCursor(0,10);
    display.write("Humid: ");
    display.write(humidity);
    display.display();
    
    
    

    //update the display

    //reset sensorCycleStart
    sensorCycleStart = millis();
    Serial.println("Sensor Read Triggered");
    if (digitalRead(13)==HIGH){
      digitalWrite(13, LOW);
      //digitalWrite(LEDlights, LOW);
    }else{
      digitalWrite(13, HIGH);
      //digitalWrite(LEDlights, HIGH);
    }
  }

  //Evaluate button pushes

  // //Lights

  // readingButtonLights = digitalRead(buttonLights);
  // // check to see if you just pressed the button
  // // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // // since the last press to ignore any noise:

  // // If the switch changed, due to noise or pressing:
  // if (readingButtonLights != lastStateButtonLights) {
  //   // reset the debouncing timer
  //   lastDebounceTimeLights = millis();
  // }

  // if ((millis() - lastDebounceTimeLights) > debounceDelay) {
  //   // whatever the reading is at, it's been there for longer than the debounce
  //   // delay, so take it as the actual current state:

  //   // if the button state has changed:
  //   if (readingButtonLights != stateButtonLights) {
  //     stateButtonLights = readingButtonLights;

  //     //toggle the LED
  //     if (stateButtonLights == HIGH){
  //       stateLights = !stateLights;
  //     }

  //     display.clearDisplay();
  //     display.setCursor(0,0);
  //     if (stateLights == HIGH){
  //       display.write("Lights On");
  //     }else{
  //       display.write("Lights Off");
  //     }
  //     display.display();
  //   }
  // }

  // //Set the lights state
  // digitalWrite(LEDlights, stateLights);

  // //save the reading. Next time through the loop, it'll be the lastButtonState:
  // lastStateButtonLights = readingButtonLights;
  
  
  // Fan Button

  readingButtonFan = digitalRead(buttonFan);
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (readingButtonFan != lastStateButtonFan) {
    // reset the debouncing timer
    lastDebounceTimeFan = millis();
  }

  if ((millis() - lastDebounceTimeFan) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (readingButtonFan != stateButtonFan) {
      stateButtonFan = readingButtonFan;

      //toggle the LED
      if (stateButtonFan == HIGH){
        stateFan = !stateFan;
      }

      display.clearDisplay();
      display.setCursor(0,0);
      if (stateFan == HIGH){
        display.write("Fan On");
      }else{
        display.write("Fan Off");
      }
      display.display();
    }
  }

  //Set the lights state
  digitalWrite(LEDFan, stateFan);
    
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastStateButtonFan = readingButtonFan;


  //  // Vent Button
  // readingButtonVent = digitalRead(buttonVent);
  // // check to see if you just pressed the button
  // // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // // since the last press to ignore any noise:

  // // If the switch changed, due to noise or pressing:
  // if (readingButtonVent != lastStateButtonVent) {
  //   // reset the debouncing timer
  //   lastDebounceTimeVent = millis();
  // }

  // if ((millis() - lastDebounceTimeVent) > debounceDelay) {
  //   // whatever the reading is at, it's been there for longer than the debounce
  //   // delay, so take it as the actual current state:

  //   // if the button state has changed:
  //   if (readingButtonVent != stateButtonVent) {
  //     stateButtonVent = readingButtonVent;

  //     //toggle the LED
  //     if (stateButtonVent == HIGH){
  //       stateVent = !stateVent;
  //     }

  //     display.clearDisplay();
  //     display.setCursor(0,0);
  //     if (stateVent == HIGH){
  //       display.write("Vent On");
  //     }else{
  //       display.write("Vent Off");
  //     }
  //     display.display();
  //   }
  // }

  // //Set the lights state
  // digitalWrite(LEDVent, stateVent);
    
  // // save the reading. Next time through the loop, it'll be the lastButtonState:
  // lastStateButtonVent = readingButtonVent;

}
