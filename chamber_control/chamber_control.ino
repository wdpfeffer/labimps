

#define lightLED PB3
#define fan1LED PB4
#define fan2LED PB5
#define swLight PA4
#define swFan1 PA2
#define swFan2 PA3
#define temp 0
#define hum 1
#define sensorInterval 10000 //read every 10
#define pbInterval 50 //pushbutton interval



byte fan1Button;
byte lastFan1Button;
byte fan2Button;
byte lastFan2Button;
byte lightsButton;
byte lastLightsButton;

unsigned long sensorCycleStart;
unsigned long sensorCycleCurrent;
unsigned long lightPush;
unsigned long fan1Push;
unsigned long fan2Push;  


void setup() {
  // put your setup code here, to run once:
  pinMode(swLight, INPUT);
  pinMode(swFan1, INPUT);
  pinMode(swFan2, INPUT);
  pinMode(lightLED, OUTPUT);
  pinMode(fan1LED, OUTPUT);
  pinMode(fan2LED, OUTPUT);
  pinMode(PC13, OUTPUT);   //board status
  
  //setup serial for debugging
  Serial.begin(9600);

 
  //Set button stats
  lastFan2Button = LOW;
  lastFan1Button = LOW;
  lastLightsButton = LOW;

  sensorCycleStart = millis();

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
    }else{
      digitalWrite(PC13, HIGH);
    }

  }

  //Check for button pushes
  lightsButton = digitalRead(swLight);
  fan1Button = digitalRead(swFan1);
  fan2Button = digitalRead(swFan2);

  if (lightsButton != lastLightsButton){
    //a push
    lightPush = millis();
  }

  if (millis()-lightPush > pbInterval){
    //looks like areal push
    //toggle the state
    if (digitalRead(lightLED)==HIGH){
      digitalWrite(lightLED,LOW);
    }else{
      digitalWrite(lightLED, HIGH);
    }
    Serial.println("Lights Button Pushed");
  }

  if (fan1Button != lastFan1Button){
    //a push
    fan1Push = millis();
  }

  if (millis()-fan1Push > pbInterval){
    //looks like areal push
    //toggle the state
    if (digitalRead(fan1LED)==HIGH){
      digitalWrite(fan1LED,LOW);
    }else{
      digitalWrite(fan1LED, HIGH);
    }
    Serial.println("Fan1 Button Pushed");
  }

  if (fan2Button != lastFan2Button){
    //a push
    fan2Push = millis();
  }

  if (millis()-fan2Push > pbInterval){
    //looks like areal push
    //toggle the state
    if (digitalRead(fan2LED)==HIGH){
      digitalWrite(fan2LED,LOW);
    }else{
      digitalWrite(fan2LED, HIGH);
    }
    Serial.println("Fan2 Button Pushed");
  }
  

}
