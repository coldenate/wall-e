/*
  Maze Completing Robot Monoscript

  This sketch contains the on-board decision making for the maze-solver project.

  by Nate Solis

  Licensed with MIT License
  Please do not use this code in an opposing DI situation. :) This is a simple request. If you are Dr. Doofenshmirtz, I cannot do anything about it.
  Copyright (c) 2022 Nate Solis
*/

#include <SparkFun_TB6612.h>
#include "DHT.h"
#include "NewPing.h"

#define AIN1 7
#define BIN1 11
#define AIN2 8
#define BIN2 12
#define PWMA 9
#define PWMB 10
#define STBY 13

// offsets? 
const int offsetA = 1;
const int offsetB = 1;

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);


#define DHTPIN A4
// West
#define trigPin_W 3 // black
#define echoPin_W 3
// East
#define trigPin_E 5
#define echoPin_E 5 // qwhite
// North (all in terms of driving forward)
#define triggerPin 6
#define echoPin 6 //oramge

const int buttonPin = A5;     // the number of the pushbutton pin

int buttonState = 0;

#define MAX_DISTANCE 400


#define DHTYPE DHT11


NewPing sonar(triggerPin, echoPin, MAX_DISTANCE);
NewPing sonarE(trigPin_E, echoPin_E, MAX_DISTANCE);
NewPing sonarW(trigPin_W, echoPin_W, MAX_DISTANCE);

float duration;
float durationE;
float durationW;
float hum;
float temp;
float distance;
float distanceE;
float distanceW;
float soundspeedms;
float soundspeedcm;

int iterations = 6;
bool debug_mode = false;
bool stdf = true; //until proven otherwise
bool Nsafe = false;
bool Esafe; 
bool Wsafe;

DHT dht(DHTPIN, DHTYPE);

void smart_turn(int reference_point, int direction) {
  if (direction == 1) {
    while (distance != round(reference_point)) {
      right(motor1, motor2, 255);
    }
  }
  if (direction == 2) {
    while (distance != round(reference_point)) {
      left(motor1, motor2, 255);
    }
  }  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  pinMode(buttonPin, INPUT);
  delay(3000);
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    debug_mode = true;    
    Serial.println();
    Serial.print("DEBUG MODE HAS BEEN ACTIVATED");
    Serial.println();
  } else {
    debug_mode = false;
  }
  
}

void loop() {



// start of sos
  hum = dht.readHumidity();
  temp = dht.readTemperature();

  soundspeedms = 331.4 + (0.606 * temp) + (0.0124 * hum);

  soundspeedcm = soundspeedms / 10000;

  // end of  sos

  duration = sonar.ping_median(iterations);
  durationE = sonarE.ping_median(iterations);
  durationW = sonarW.ping_median(iterations);

// gathering distance

  distance = (duration / 2) * soundspeedcm;
  distanceE = (durationE / 2) * soundspeedcm;
  distanceW = (durationW / 2) * soundspeedcm;
  // end of gatehring distance

  if (debug_mode == true) {

    Serial.println("Sound :");
    Serial.print(soundspeedms);
    Serial.print(" m/s, ");
    Serial.print("Humidity:");
    Serial.print(hum);
    Serial.print("%, Temp:");
    Serial.print(temp);
    Serial.println(" C | Distances: ");
  }


  Serial.println();
  Serial.print("\t\t\t\t\t\t\t\t");
  // tht was a bunch f tbats????

  if (distance >= 400 || distance <= 10) {
    Nsafe = false;
    if (debug_mode == true) {
    Serial.print("NOut of range | ");}
  }

  else {
    Nsafe=true;
    if (debug_mode == true) {
    Serial.print(distance);
    Serial.print(" cm NORTH | ");}
  }
  if (distanceE >= 400 || distanceE <= 10) {
    Esafe = false;
    if (debug_mode == true) {
    Serial.print("EOut of range | ");
  }}

  else {
    Esafe=true;
    if (debug_mode == true) {
    Serial.print(distanceE);
    Serial.print(" cm EAST | ");}
  }
  if (distanceW >= 400 || distanceW <=10) {
    Wsafe=false;
    if (debug_mode == true) {
    Serial.println("WOut of range | ");
  }}

  else {
    Wsafe=true;
    if (debug_mode == true) {
    Serial.print(distanceW);
    Serial.print(" cm WEST | ");}
  }
 
  if (Nsafe == true) {
    stdf=true;
    Serial.println("I am driving forward");
    motor1.drive(150);
    motor2.drive(150);
  }
  if (Nsafe==false) {
    Serial.println("Wait crap there is dead end man cmon. Traffic jammmm");
    stdf=false;
    // at this point, the decision tree will decide if we should turn left or right
    if (Wsafe == true) {
      // turn left
      Serial.print("Turn leftt");
      if (distance != round(distanceW)) {
      left(motor1, motor2, 255);}
      Nsafe = true;
    }
    if (Esafe == true) {
      // turn right
      Serial.print("Turn right");
      if (distance != round(distanceE)) {
      right(motor1, motor2, 255);
    }
      Nsafe = true;
    }
  }

  
}
