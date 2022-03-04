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


#define DHTPIN 2
// West
#define trigPin_W 3 // black
#define echoPin_W 3
// East
#define trigPin_E 5
#define echoPin_E 5 // qwhite
// North (all in terms of driving forward)
#define triggerPin 6
#define echoPin 6 //oramge
#define buttonPin 4

// const int buttonPin = 4;     // the number of the pushbutton pin

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
int EDynaThreshLow = 8;
int EDynaThreshHigh = 14;
int WDynaThreshLow = 8;
int WDynaThreshHigh = 14;
bool debug_mode = false;
bool stdf = true; //until proven otherwise
bool Nsafe = false;
bool Esafe; 
bool Wsafe;
bool just_turned = false;
bool driving = false;
bool is_turningL = false;
bool is_turningR= false;
float turning_dest;
bool ignore_north = false; // this is for ramming objects when in open field.
bool inRange(unsigned low, unsigned high, unsigned x){
  return (low <= x && x <= high);     
}

DHT dht(DHTPIN, DHTYPE);

void dynathresh() {
  /*
  This function may be iffy, but it should improve accuracy. 
  dynamic maze wall proximity threshold detection.
  This function will be designed to run at setup. We will need to place it in the maze walls at setup unless we want to advance the function
  and make it drive into the maze, see the ultras change drastically (negativly), then run this calibration. This is a good idea, but the time I have to implement it is short.
  */
  if (debug_mode==true){
    return;
  }
  brake(motor1, motor2);
  delay(1000); // this makes sure it can truly calibrate. 
  find_prox();
  EDynaThreshLow = round(distanceE-4);
  EDynaThreshHigh = round(distanceE+4);
  WDynaThreshLow = round(distanceW-4);
  WDynaThreshHigh = round(distanceW+4);
}


void setup() {
  // put your setup code here, to run once:
  dht.begin();
  pinMode(buttonPin, INPUT);
  buttonState = digitalRead(buttonPin);
  delay(2000);
  if (buttonState == HIGH) {
    debug_mode = true;    
    Serial.begin(9600);
    Serial.println();
    Serial.print("DEBUG MODE HAS BEEN ACTIVATED - TURNING OFF MOTORS");
    Serial.println();
    // Serial.print("Establish Edynathresh as");
    // Serial.println(EDynaThresh);
    // Serial.print("Establish WdDynaThresh as");
    // Serial.println(WDynaThresh);
    
  } else {
    // debug_mode = false;
    dynathresh(); // the dynamic refresh generator is down here as to not confuse the berserk mode (which needs to ignore all law and life. Berserk mode is like a breakdown recovery, 
    // you need to forget all your pain, and ram into all your problems.)
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

void turn_left(bool first, float distanceW) {
  is_turningL = true;
  if (first==true){
    // Serial.println("Reset turning dest");
    turning_dest = distanceW;
  }
  delay(100);
  left(motor1,motor2, 250);
  
  find_prox();
  // Serial.println("The turning desitnation is:");
  Serial.print(turning_dest);
  // Serial.println(" But the north is");
  // delay(250);
  // brake(motor1,motor2);
  
  Serial.print(distance);
  inRange(turning_dest-2, turning_dest+2, distance)? is_turningL = false: is_turningL = true;
  
  if (is_turningL == true){
    turn_left(false, distanceW);
    
    }
  if (is_turningL==false){
  brake(motor1,motor2);
  delay(250);
  just_turned = true;
  is_turningL = false;  
  Serial.print("WE HAVE TURNED LEFt");
  
  
  }
  }
void turn_right(bool first, float distanceE) {
  is_turningR = true;
  if (first==true){
    Serial.println("Reset turning dest");
    turning_dest = distanceE;
  
  }
  delay(100);
  right(motor1,motor2, 250);
 
  find_prox();
  // Serial.println("The turning desitnation is:");
  // Serial.print(turning_dest);
  // Serial.println(" But the north is");
  // delay(250);
  // brake(motor1,motor2);
  
  Serial.print(distance);
  inRange(turning_dest-2, turning_dest+2, distance)? is_turningR = false: is_turningR = true;
  
  if (is_turningR == true){
    turn_right(false, distanceW);
    
    }
  if (is_turningR==false){
  brake(motor1,motor2);
  delay(250);
  just_turned = true;
  is_turningR = false;
  Serial.print("WE HAVE TURNED RIGHt");
  
  
  }
  }

void loop() {
  find_prox();
  // if (is_turningL==true){
  //   turn_left(false, distanceW);
  // }    
  // if (is_turningR==true){
  //   turn_right(false, distanceE);
  // }
    // possible recursion
  
    


  if (just_turned == false){
  /* if we have not just turned, we need to do scanning. 
  If we have turned, we know the instance of the loop that chose to turn knew the turn lane was safe,
  so this is a way to carry that knowledge over to the second in-line instance/iteration of the loop. 
  */

    
    if (debug_mode == true) {
      Serial.print(distance);
      Serial.print(" cm NORTH | ");
      Serial.print(distanceE);
      Serial.print(" cm EAST | ");
      Serial.print(distanceW);
      Serial.print(" cm WEST | ");
  }
    if (driving == true) {
      if (distance >= 400 || distance <= 21) {// || is like or in python 
        Nsafe = false;
        brake(motor1,motor2);
        back(motor1,motor2,150);
        delay(350);
        brake(motor1,motor2);

        Serial.println("Dont drive north");
        driving = false;

      }
      if (distance >= 20) {
        Nsafe = true;
      }
  }
    else {
    if (distance >= 400 || distance <= 21) {// || is like or in python 
      Nsafe = false;
    }
    if (distance >= 21) {
      Nsafe = true;
    }
    if (distanceE >= 400 || distanceE <= EDynaThreshLow) {// || is like or in python 
      Esafe = false;
    }
    if (distanceE >= EDynaThreshHigh) {
      Esafe = true;
    }
    if (distanceW >= 400 || distanceW <= WDynaThreshLow) {// || is like or in python 
      Wsafe = false;
    }
    if (distanceW >= WDynaThreshHigh) {
      Wsafe = true;
    }
    }
  }
  if (just_turned == true) {
    Serial.println("I just turned, so it has to be safe to go North.");
    forward(motor1,motor2,250);
    delay(1000);
    if (just_turned == true && distanceE <= 20 && distanceW <= 20) { //if we are driving and the sides are in range of walls
      dynathresh();
  } 
    Nsafe = true;
    Wsafe = false;
    Esafe = false;
    just_turned = false;
    
  }// just turned trumnps everything
  if (Nsafe == false) {
    // motor1.brake();
    // motor2.brake();
    // Serial.println("Dont drive north");
    // driving = false;
    // I moved the stopper function to the very top so it can be faster at realizing when to stop.

  

    if (Wsafe == true && Esafe == false && Nsafe == false) {
      Serial.println("Turn left");
      delay(500);
      find_prox();
      turn_left(true, distanceW);
    }
    if (Wsafe == false && Esafe == true  && Nsafe == false) {
      Serial.println("Turn right");
      delay(500);
      find_prox();
      turn_right(true, distanceE);
    }
    if (Wsafe == false && Esafe == false && Nsafe == false) {
    Serial.println("dead end"); //berskerk mode
    // back(motor1, motor2, 150);
    // delay(2000);
    }
    if (Wsafe == true && Esafe == true && Nsafe == false) {
      Serial.println("fork in the road that I can't yet handle. I can turn left and right.");
      if (debug_mode == true){
        forward(motor1,motor2,250);
        delay(5000);}
      // back(motor1, motor2, 150);
      // delay(2000);
      return;
    }
  }
  if (Nsafe == true && Wsafe == false && Esafe == false) {
    // drive forward
    if (debug_mode == true){
      forward(motor1,motor2,250);
      delay(5000);}
    forward(motor1,motor2,150);
    Serial.println("Drive North"); 
    driving = true;
    
    
  }
  if (Wsafe == true && Esafe == true && Nsafe == true) {
    Serial.println("Driving north in an open field"); //berskerk mode
    if (debug_mode==true){
    forward(motor1,motor2,250);
    delay(5000);
  }
    // forward(motor1, motor2, 250);
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
  


  Serial.println();
  Serial.print("\t\t\t\t\t\t\t\t");
  }
  
}