/*
  Maze Completing Robot Monoscript

  This sketch contains the on-board decision making for the maze-solver project.

  by Nate Solis

  Licensed with MIT License
  Please do not use this code in an opposing Destination Imagination situation. :) This is a simple request. If you are Dr. Doofenshmirtz, I cannot do anything about it.
  Copyright (c) 2022 Nate Solis
*/
#include <SparkFun_TB6612.h>
#include "DHT.h"
#include "NewPing.h"
// motor driver pins
#define AIN1 7
#define BIN1 11
#define AIN2 8
#define BIN2 12
#define PWMA 9
#define PWMB 10
#define STBY 13
//motor driver offsets
const int offsetA = 1;
const int offsetB = 1;

// Temperature Sensor Pin and Type

#define DHTPIN 2
#define DHTYPE DHT11

// Define Ultrasonic Sensor Pins
//West
#define trigPinWest 3
#define echoPinWest 3 // they are the same because the script supports a dual-channel type flow for sending and receiving pulses. 
//East
#define trigPinEast 5
#define echoPinEast 5
//North (Prioritized driving direction)
//North is also in context of the robot, not the actual magnetic fields of earth. Although the script may include a magnetometer.
#define trigPinNorth 6
#define echoPinNorth 6
//Button for actiting debug mode
#define buttonPin 4

int buttonState=0; // a value for the default (OFF) button state | this represents the button NOT completing a circuit flow. 

#define MAX_DISTANCE 400 // I AM ON MAXIMUM RENDER DISTANCE, AND I STILL CAN'T FIND WHO ASKED (Max distance to care about in the world of ultrasonic sound)


// dump truck of floats

float durationN;
float durationE;
float durationW;
float hum; // humidity
float temp; //temperature (both derviedfrom our trmp sensoirkrfgksdjf)
float distanceN;
float distanceE;
float distanceW;
float soundspeedms; //
float soundspeedcm; // 

int iterations = 7; // amount of times we poke our ultrsonic sensors. ex. if iterations =7 (whats the distance bro,whats the distance bro,whats the distance bro,whats the distance bro,whats the distance bro,whats the distance bro,whats the distance bro,)
//default dynamic threshold generations if the threshold generation doesn'texecute
int EDynaThreshLow = 8;
int EDynaThreshHigh = 14;
int WDynaThreshLow = 8;
int WDynaThreshHigh = 14;
// decision tree manuplation 
bool berserk_mode = false;
bool debug_mode = true; // effects serial output 
bool stdf = true; //until proven otherwise
bool Nsafe = false;
bool Esafe; 
bool Wsafe;
bool just_turned = false;
bool driving = false;
bool is_turningL = false;
bool is_turningR= false;
float turning_dest;
bool ignore_north = false; 
bool inRange(unsigned low, unsigned high, unsigned x){
  return (low <= x && x <= high);     
} // this is used as a quick range checkthst can be ombined with ? : 

// Object Declarations
//motor driver motor objects
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

// Newping Library Object Declarations

NewPing sonarN(trigPinNorth, echoPinNorth, MAX_DISTANCE);
NewPing sonarE(trigPinEast, echoPinEast, MAX_DISTANCE);
NewPing sonarW(trigPinWest, echoPinWest, MAX_DISTANCE);

DHT dht(DHTPIN, DHTYPE);

void dynathresh() {
  return;
}

void turn(bool initialize, float target, char direction) {return;}

int sos() {
  hum = dht.readHumidity();
  temp = dht.readTemperature();

  soundspeedms = 331.4 + (0.606 * temp) + (0.0124 * hum);

  soundspeedcm = soundspeedms / 10000;
  return soundspeedcm;
}

void find_prox() {
  /* 
  Runs the proximity checks
  we can call this function at anytime to improve the accuracy of our time sitatuion.
  */
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
void setup () {return;}

void loop () {

  if (just_turned == true){
    
  }

}