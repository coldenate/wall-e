
#include <MeDCMotor.h>
#include <MeEEPROM.h>                                                                                                                                                                                                                                                                                                                                                                                                            cv
#include <MeEncoderMotor.h>
#include <MeEncoderNew.h>
#include <MeEncoderOnBoard.h>

// #include <Motor_PID.h>

const int motorPin1 = 2; 
// #define ENCA 33
const int motorPin2 = 3;
const int switchPin = 4;

int switchState = 0;



void setup() {
  Serial.begin(9600);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(switchPin, INPUT);
}

void loop() {
  switchState= digitalRead(switchPin);
  
  if (switchState == HIGH) {
    // turn motor on:
    Serial.println("pressed");
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin1, HIGH);
  } else {
    // turn motor off:
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin1, LOW);
  }
}
