// include the servo library
#include <Servo.h>
Servo myServo;  // create a servo object 

// set pin numbers
const int buttonPin = 2;      // the number of the pushbutton pin
const int relayPin = 4;       // the number of the relay coil pin
const int servoPin = 11;      // the number of the LED pin

// servo, relay, and button state
int servoState = 100;         // the current (analog) state of the output pin
int relayState = LOW;
int buttonState = LOW;        // the current reading from the input pin

// last button and servo state 
int lastButtonState = LOW;    // the previous reading from the input pin
int lastServoState = 0;       // the previous servo state

// flag to store if servo is dropping/dropped
boolean servoDrop = false;    // drop the servo?

// timers for servo, loop delta, and debouncing
long servoTimer = 2400;           // how long to move the servo into place
long lastLoop = 0;
long lastDebounceTime = 0;        // the last time the output pin was toggled
long debounceDelay = 50;          // the debounce time; increase if the output flickers

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(relayPin, OUTPUT);
  myServo.attach(11);             // attaches the servo on pin 9 to the servo object// set the servo:
  myServo.write(servoState);
  Serial.begin(9600);             // open a serial connection to your computer
}

void loop() {  
  
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle if the new button state is HIGH
      if (buttonState == HIGH) {
        // reset if servo is already moving/moved
        if (servoDrop) {     
          servoTimer = 2400;
          servoDrop = false;
          relayState = LOW;
        } else {
          servoDrop = true;
        }
      }
    }
  }
  
  // save last servo state
  lastServoState = servoState;
  
  // count down milliseconds from servo timer
  if (servoDrop) {    
    if (servoTimer > 0) {
      servoState = map(servoTimer, 0, 2400, 30, 100);
      servoTimer -= millis() - lastLoop;   
    } else {
      // light up
      relayState = HIGH;
    }
  } else {
    servoState = 100; 
  }
  
  if (lastServoState != servoState) {
    // set the servo:
    myServo.write(servoState);
  }
  
  // write to relay pin
  digitalWrite(relayPin, relayState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading; 
  lastLoop = millis();
}


