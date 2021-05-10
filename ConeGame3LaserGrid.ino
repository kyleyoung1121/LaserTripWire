//General Variables
int laserPin = 7;
int doorBellPin = 6;
bool PIRtripped = false;
long unsigned int timeSeen;
int laserTurnOffDelay = 15 * 1000; //Lasers turn on for 15 seconds
bool firstLaserLoop = true;

//LDR Variables
int lightThreshold = 250; //Doesn't need to be much. The LDR sits around 1020-1023 pretty comfortably.
int LDRcalibrationTime = 3000; //Gives the laser a bit to rest on the LDR and build up resistance before avg is found
int LDRpin = 0;
int LDRValue = 0;
int loopCount = 0;
int loopStartingAvg = 0;
int LEDpin = 8;

//PIR Variables
int calibrationTime = 15; //the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int PIRdelay = 1500;
int pirPin = 3; //the digital pin connected to the PIR sensor's output  

//SETUP 
void setup(){ 
  Serial.begin(9600);
  pinMode(laserPin, OUTPUT);
  pinMode(doorBellPin, OUTPUT);
  pinMode(pirPin, INPUT); 
  pinMode(LEDpin, OUTPUT);
  digitalWrite(pirPin, LOW); 
  digitalWrite(laserPin, HIGH); //Writing high here, because the relay is funky.
  digitalWrite(doorBellPin, HIGH); //Writing high here, because the relay is funky.
  //give the sensor some time to calibrate 
  Serial.print("calibrating sensor "); 
  for(int i = 0; i < calibrationTime; i++){ 
    Serial.print("."); 
    delay(1000); 
  } 
  Serial.println(" done"); 
  Serial.println("SENSOR ACTIVE"); 
  digitalWrite(LEDpin, HIGH);
  delay(50); 
  digitalWrite(LEDpin, LOW);
  delay(50);
  digitalWrite(LEDpin, HIGH);
  delay(50); 
  digitalWrite(LEDpin, LOW);
  delay(50);
} 

//This code runs when a beam is tripped
void beamTrip() {
  digitalWrite(doorBellPin, LOW);
  digitalWrite(LEDpin, HIGH);
  delay(1000);
  digitalWrite(doorBellPin, HIGH);
  digitalWrite(LEDpin, LOW);
  delay(1000);
}

//LOOP 
void loop(){ 
  /*  Handy dandy led blink to show how often the loop checks for change. Save battery?
  digitalWrite(LEDpin, HIGH);
  delay(25); 
  digitalWrite(LEDpin, LOW);
  delay(25);
  */
  if (!PIRtripped) {
    //Code for when the PIR hasn't been tripped yet
    if(digitalRead(pirPin) == HIGH){ 
      //the led visualizes the sensors output pin state 
      if(!PIRtripped){ 
        //makes sure we wait for a transition to LOW before any further output is made: 
        PIRtripped = true; 
        Serial.println("---"); 
        Serial.print("motion detected at "); 
        Serial.print(millis()/1000); 
        Serial.println(" sec");
        timeSeen = millis(); 
        delay(50); 
      }  
    }
    delay(PIRdelay);
  } else {
    //Code for once the PIR is tripped and shortly after
    if (millis() - timeSeen > laserTurnOffDelay) { //Check to see if the laser active time is up
      //If the laser is to be turned off, check for additional motion, so that it doesn't just click off and then back on.
      if(digitalRead(pirPin) == HIGH){
        Serial.println("---"); 
        Serial.print("continued motion at "); 
        Serial.print(millis()/1000); 
        Serial.println(" sec");
        timeSeen = millis();
      } else {
        PIRtripped = false; //Prepare to wait for motion, exiting laser loop
        digitalWrite(laserPin, HIGH); //Turn off the laser
        firstLaserLoop = true; //Sets this for next time 
      }
    } else {
      if (firstLaserLoop) {
        digitalWrite(laserPin, LOW);  //Turn on laser for the first time
        delay(LDRcalibrationTime);
        for (int i = 0; i < 10; i++) {
          LDRValue = analogRead(LDRpin);
          loopStartingAvg += LDRValue; //Build total of light input
          delay(50);
        }
        loopStartingAvg /= 10; //Divide to get average light level to compare to
        Serial.println("Avg: " + String(loopStartingAvg)); //Log the light average upon a trip
        firstLaserLoop = false;
      } else {
        //The code should spend most of the time looping here, checking the light level of the LDR
        LDRValue = analogRead(LDRpin);
        if (LDRValue < loopStartingAvg - lightThreshold) {
          Serial.println("~~~~~~~~~~");
          Serial.println("TRIPPED!!!");
          Serial.print("LDRValue: ");
          Serial.println(LDRValue);
          Serial.print("Value to beat: ");
          Serial.println(loopStartingAvg - lightThreshold);
          beamTrip(); 
        }  
      }
      delay(150); //Add delay so that it won't check again for a beam broken/time to turn off quite so rapidly.
    }
  } 
}
