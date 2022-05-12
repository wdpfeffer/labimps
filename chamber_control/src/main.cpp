#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


#define buttonLights 2  //input of the light button
#define buttonVent 3   //input of the vents button
#define buttonFan 5     //input for the fan button
#define LEDlights A0
#define LEDVent A1
#define LEDFan A2
#define temp 0
#define hum 1
#define sensorInterval 60000 //read every 10
#define debounceDelay 50 //pushbutton interval

#define DHTPIN 6  //digital pin for temp and humidity readings
#define DHTTYPE DHT22  //The type of dht probe we are using

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define maxcount 15 //maximum number of displayed cycle and array lenthgs
#define multiplier 8 //x axis multiplier
#define tempMin 20.0  //lowest temperature for gauge
#define tempMax 80.0  //Max temperature for gauge
#define humMin 0  //minimum humidity for gauge
#define humMax 50.0 //Max humidity for gauge

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

int ncount = 0;  // number of cycles
int tempA[maxcount];
int humA[maxcount];

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
  if (millis()-sensorCycleStart > sensorInterval || ncount == 0){
    //Read temp and humidity
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    
    //Draw Axes
    display.clearDisplay();
    //draw x axis
    display.drawLine(0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1,
    SCREEN_WIDTH-1, SSD1306_WHITE);

    //draw y axis
    display.drawLine(0, 0, 0, SCREEN_WIDTH-1, SSD1306_WHITE);
    display.display();

    if (ncount < maxcount){
      //add directly to array
      tempA[ncount] = t;
      humA[ncount] = h;
    }else{
      for (int i = 0; i < maxcount-1; i++){
        tempA[i] = tempA[i+1];
        humA[i] = humA[i+1];
      }
      tempA[maxcount-1] = t;
      humA[maxcount-1] = h;
    }

    for (int n=0; n < ncount; n++){
      // scale the temps and humidity so it fits on plot
      // Assume a rangeo of 20 to 80 C for Temperature
      // and 0 to 50 for rel humidity

      int yT = (SCREEN_HEIGHT-1)-(tempA[n]-tempMin)/(tempMax-tempMin)*(SCREEN_HEIGHT-1);
      int yH = (SCREEN_HEIGHT-1)-(humA[n]-humMin)/(humMax-humMin)*(SCREEN_HEIGHT-1);

      //plot it
      display.fillCircle(n*multiplier, yH, 2, SSD1306_WHITE);
      display.drawCircle(n*multiplier, yT, 2, SSD1306_WHITE);
    }
    display.display();

    //print out temp and humidity as text
    display.setTextSize(1);
    display.setCursor(10,0);
    
    char temperature[5];
    dtostrf(t,4,1,temperature);
    
    display.write("T: ");
    display.write(temperature);
    display.display();

    char humidity[5];
    dtostrf(h,4,1,humidity);

    display.setCursor(10,10);
    display.write("H: ");
    display.write(humidity);
    display.display();

    //update counter if needed
    if (ncount < maxcount){
      ncount++;
    }
    
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

  {//Lights

  readingButtonLights = digitalRead(buttonLights);
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (readingButtonLights != lastStateButtonLights) {
    // reset the debouncing timer
    lastDebounceTimeLights = millis();
  }

  if ((millis() - lastDebounceTimeLights) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (readingButtonLights != stateButtonLights) {
      stateButtonLights = readingButtonLights;

      //toggle the LED
      if (stateButtonLights == HIGH){
        stateLights = !stateLights;
      }

      display.clearDisplay();
      display.setCursor(0,0);
      if (stateLights == HIGH){
        display.write("Lights On");
      }else{
        display.write("Lights Off");
      }
      display.display();
    }
  }
  
  //Set the lights state
  digitalWrite(LEDlights, stateLights);

  //save the reading. Next time through the loop, it'll be the lastButtonState:
  lastStateButtonLights = readingButtonLights;
  }
  
  {//Fan

  readingButtonFan = digitalRead(buttonFan);
  Serial.print(readingButtonFan);
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

  //save the reading. Next time through the loop, it'll be the lastButtonState:
  lastStateButtonFan = readingButtonFan;
  }

  {// Vent Button
  readingButtonVent = digitalRead(buttonVent);
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (readingButtonVent != lastStateButtonVent) {
    // reset the debouncing timer
    lastDebounceTimeVent = millis();
  }

  if ((millis() - lastDebounceTimeVent) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (readingButtonVent != stateButtonVent) {
      stateButtonVent = readingButtonVent;

      //toggle the LED
      if (stateButtonVent == HIGH){
        stateVent = !stateVent;
      }

      display.clearDisplay();
      display.setCursor(0,0);
      if (stateVent == HIGH){
        display.write("Vent On");
      }else{
        display.write("Vent Off");
      }
      display.display();
    }
  }

  //Set the lights state
  digitalWrite(LEDVent, stateVent);
    
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastStateButtonVent = readingButtonVent;
  }
  //delay(2000);
}
