

#define lightLED PB3
#define swLight PA2
#define temp 0
#define hum 1
#define sensorInterval 10000 //read every 10
#define pbInterval 50 //pushbutton interval



int lightsButton;
int lastLightsButton;

unsigned long sensorCycleStart;
unsigned long sensorCycleCurrent;
unsigned long lightPush;


void setup() {
  // put your setup code here, to run once:
  pinMode(swLight, INPUT_PULLDOWN);
  
  pinMode(lightLED, OUTPUT);
  pinMode(PC13, OUTPUT);   //board status
  
  //setup serial for debugging
  Serial.begin(9600);

 
  //Set button stats
  lastLightsButton = LOW;

  sensorCycleStart = millis();
  lightPush = 0;

}

void loop() {
  // put your main code here, to run repeatedly:

  //Determine if we need a sensor read
  if (millis()-sensorCycleStart > sensorInterval){
    //Read temp and humidity

    //update the display

    //reset sensorCycleStart
    sensorCycleStart = millis();
    Serial.println("Sensor Read Triggered");
    if (digitalRead(PC13)==HIGH){
      digitalWrite(PC13, LOW);
      //digitalWrite(lightLED, LOW);
    }else{
      digitalWrite(PC13, HIGH);
      //digitalWrite(lightLED, HIGH);
    }
  }

  if (digitalRead(swLight)==HIGH && millis()-lightPush > pbInterval){
    if (digitalRead(lightLED)==HIGH){
      digitalWrite(lightLED, LOW);
    }else{
      digitalWrite(lightLED, HIGH);
    }
    lightPush=millis();   
  }

//  //Check for button pushes
//  lightsButton = digitalRead(swLight);
//
//  if (lightsButton != lastLightsButton){
//    //a push
//    lightPush = millis();
//  }
//
//  if (millis()-lightPush > pbInterval){
//    //looks like areal push
//    //toggle the state
//    if (digitalRead(lightLED)==LOW){
//      digitalWrite(lightLED,HIGH);
//    }else{
//      digitalWrite(lightLED, LOW);
//    }
//    Serial.println("Lights Button Pushed");
//  }

}
