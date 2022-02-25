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
bool just_turned = false;
bool driving;
bool is_turning;

DHT dht(DHTPIN, DHTYPE);


void setup() {
  // put your setup code here, to run once:
  dht.begin();
  pinMode(buttonPin, INPUT);
  delay(3000);
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    debug_mode = true;    
    Serial.begin(9600);
    Serial.println();
    Serial.print("DEBUG MODE HAS BEEN ACTIVATED");
    Serial.println();
  } else {
    debug_mode = false;
  }
  
}

int sos() {
  hum = dht.readHumidity();
  temp = dht.readTemperature();

  soundspeedms = 331.4 + (0.606 * temp) + (0.0124 * hum);

  soundspeedcm = soundspeedms / 10000;
  return soundspeedcm;
}

void find_prox() {
  sos();
  
  duration = sonar.ping_median(iterations);
  durationE = sonarE.ping_median(iterations);
  durationW = sonarW.ping_median(iterations);

// gathering distance

  distance = (duration / 2) * soundspeedcm;
  distanceE = (durationE / 2) * soundspeedcm;
  distanceW = (durationW / 2) * soundspeedcm;
  // end of gatehring distance
 
}

void loop() {

  if (just_turned == true) {
    just_turned = false;
}

  find_prox();
  if (debug_mode == true) {
    Serial.print(distance);
    Serial.print(" cm NORTH | ");
    Serial.print(distanceE);
    Serial.print(" cm EAST | ");
    Serial.print(distanceW);
    Serial.print(" cm WEST | ");
}
  if (driving == true) {
    if (distance >= 400 || distance <= 15) {// || is like or in python 
      Nsafe = false;
    }
    if (distance >= 15) {
      Nsafe = true;
    }
}
  else {
  if (distance >= 400 || distance <= 15) {// || is like or in python 
    Nsafe = false;
  }
  if (distance >= 15) {
    Nsafe = true;
  }
  if (distanceE >= 400 || distanceE <= 15) {// || is like or in python 
    Esafe = false;
  }
  if (distanceE >= 15) {
    Esafe = true;
  }
  if (distanceW >= 400 || distanceW <= 15) {// || is like or in python 
    Wsafe = false;
  }
  if (distanceW >= 15) {
    Wsafe = true;
  }
  }
  if (just_turned == true) {
    Serial.println("I just turned, so it has to be safe to go North.");
  }// just turned trumnps everything
  if (Nsafe == false) {
    motor1.brake();
    motor2.brake();
    Serial.println("Dont drive north");
    driving = false;

  

    if (Wsafe == true && Esafe == false && Nsafe == false) {
      Serial.println("Turn left");
    }
    if (Wsafe == false && Esafe == true  && Nsafe == false) {
      Serial.println("Turn right");
    }
    if (Wsafe == false && Esafe == false && Nsafe == false) {
    Serial.println("dead end"); //berskerk mode
    }
    if (Wsafe == true && Esafe == true && Nsafe == false) {
      Serial.println("fork in the road that I can't yet handle. I can turn left and right.");
    }
  }
  if (Nsafe == true && Wsafe == false && Esafe == false) {
    // drive forward
    motor1.drive(150); 
    motor2.drive(150);
    Serial.println("Drive North"); 
    driving = true;
    
  }
  if (Wsafe == true && Esafe == true && Nsafe == true) {
    Serial.println("Driving north in an open field"); //berskerk mode
  }
  
  

  // this was for north

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
  
  
}
