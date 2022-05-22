

/*
  Maze Completing Robot Monoscript

  This sketch contains the on-board decision making for the maze-solver project.

  by Nathan Solis

  Licensed with MIT License
  Please do not use this code in an opposing Destination Imagination situation. :) This is a simple request. If you are Dr. Doofenshmirtz, I cannot do anything about it.
  Copyright (c) 2022 Nathan Solis and the BBASH HCA Team
*/

/* TODO: Implement
- Right-Hand Rule algorithm
- Clean Up turning ai
- read some articles for inspiration or something
*/
#include "Arduino.h"
#include "SparkFun_TB6612.h"
#include "DHT.h"
#include "NewPing.h"
// motor driver pins
#define PWMA 9
#define AIN2 8
#define AIN1 7
#define STBY 13
#define BIN1 11
#define BIN2 12
#define PWMB 10

// Temperature Sensor Pin and Type

#define DHTPIN 2
#define DHTYPE DHT11

// Define Ultrasonic Sensor Pins

// West
#define trigPinWest 3 // green and blue
#define echoPinWest 3 // they are the same because the script supports a dual-channel type flow for sending and receiving pulses.
// East
#define trigPinEast 5
#define echoPinEast 5
// North (Prioritized driving direction)
// North is also in context of the robot, not the actual magnetic fields of earth. Although the script may include a magnetometer.

#define trigPinSouth 4
#define echoPinSouth 4

#define trigPinNorth 6
#define echoPinNorth 6
// Button for actiting debug mode

#define buttonPin A5

#define MAX_DISTANCE 400 // I AM ON MAXIMUM RENDER DISTANCE, AND I STILL CAN'T FIND WHO ASKED (Max distance to care about in the world of ultrasonic sound)

int buttonState = 0; // a value for the default (OFF) button state | this represents the button NOT completing a circuit flow.
unsigned long timeCurrentvec = 0;
float previous_vector;
float durationN;
float durationE;
float durationW;
float distances;
float durationS;
float hum;  // humidity
float temp; // temperature (both derviedfrom our trmp sensoirkrfgksdjf)
float distanceN;
float distanceNtemp;
float distanceE;
float distanceW;
float distanceS;
float soundspeedms; //
float soundspeedcm; //
float difference;
float is_turning1 = false;
float is_turning2 = false;
float is_turning11 = false;
float is_turning22 = false;
int count = 0;
// adjustments
const int iterations = 6; // amount of times we poke our ultrsonic sensors.

const float turnBuffer = 0.15;

// constant speeds
const int turning_speed = 175;

int whileiter = 0;

// motor driver offsets
const int offsetA = 1;
const int offsetB = 1;

// default dynamic threshold generations if the threshold generation doesn'texecute

int EDynaThreshLow = 8;
int EDynaThreshHigh = 14;
int WDynaThreshLow = 8;
int WDynaThreshHigh = 14;

// decision tree manuplation
bool berserk_mode = false;
bool debug_mode = true; // effects serial output
bool stdf = true;       // until proven otherwise
bool corrected = 0;
bool Nsafe = false;
bool Esafe;
bool Wsafe;
bool Ssafe;
bool just_turned = false;
bool driving = false;

float turning_dest;
float turning_destS;
bool ignore_north = false;

bool inRange(unsigned low, unsigned high, unsigned x)
{
  return (low <= x && x <= high);
} // this is used as a quick range checkthst can be ombined with ? :

// Object Declarations
// motor driver motor objects
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

// Newping Library Object Declarations

NewPing sonarN(trigPinNorth, echoPinNorth, MAX_DISTANCE);
NewPing sonarS(trigPinSouth, echoPinSouth, MAX_DISTANCE);
NewPing sonarE(trigPinEast, echoPinEast, MAX_DISTANCE);
NewPing sonarW(trigPinWest, echoPinWest, MAX_DISTANCE);

unsigned long prevTimeprox = 0;
unsigned long timeDelayprox = 2;

unsigned long prevTimevec = 0;
unsigned long timeDelayvec = 2;

unsigned int pingSpeed = 35; // 20 times as second
unsigned long pingTimer;

unsigned long prevTimedrive = 0;
unsigned long timeDelaydrive = 4;

DHT dht(DHTPIN, DHTYPE);
// Function that runs actions necesary to dynamically generate a threshold for maze hallway reconsideration.
void dynathresh()
{
  if (berserk_mode == true)
  {
    return; // for now, We doNOT need threshold generatgion if we are going berserk. This is because this function has shown signs of mapping an imaginary maze for the robot to travel in.
  }
  brake(motor1, motor2);

  EDynaThreshLow = round(distanceE - 4);
  EDynaThreshHigh = round(distanceE + 4);
  WDynaThreshLow = round(distanceW - 4);
  WDynaThreshHigh = round(distanceW + 4);
}

// void detect_stray_vector(float prev_vector, float new_vector, bool is_left)
// {
//   if (new_vector - prev_vector < 101 && new_vector - prev_vector > 1)
//   {
//     if (is_left == true)
//       is_turning1 = true;
//   }
//   if (is_left == false)
//   {
//     is_turning22 = true;
//   }
// }

void turn(bool initialize, bool is_left = false)
{
  // previous_vector = distanceN; // THIS IS BEFORE WE RUN ANOTHER PING. WE ARE TRYING TO SOLVE THE VECTOR REFLECTION ISSUE.
  if (is_left == true)
  {
    is_turning1 = true;
  }
  if (is_left == false)
  { // "if we are turning right"
    is_turning22 = true;
  }
  if (initialize == true)
  {
    if (is_left == true)
    {
      turning_dest = distanceW;
      turning_destS = distanceE;
    }
    if (is_left == false)
    {
      turning_dest = distanceE;
      turning_destS = distanceW;
    }
  }
  whileiter = 0;
  while (true)
  {

    find_N();
    Serial.println("while loop iteration:");
    whileiter = whileiter + 1;
    Serial.print(whileiter);
    Serial.println("N | DEST");
    Serial.print(distanceN);
    Serial.print("||");
    Serial.print(turning_dest);
    // if the codeblock above fails, try moving it out of the while loop, especiallyif the functions go with delays.

    if (is_left == true)
    {
      inRange(turning_dest - turnBuffer, turning_dest + turnBuffer, distanceN) ? is_turning1 = false : is_turning1 = true;
      inRange(turning_destS - turnBuffer, turning_destS + turnBuffer, distanceS) ? is_turning2 = false : is_turning2 = true;
      // detect_stray_vector(previous_vector, distanceN, true);
      if (is_turning1 == true && is_turning2 == true)
      {
        if (is_left == true)
        {
          left(motor1, motor2, turning_speed);
          Serial.println("Turning left...");
        }
        if (is_left == false)
        {
          right(motor1, motor2, turning_speed);
          Serial.println("Turning right...");
        }
      }
      if (is_turning1 == false)
      {
        brake(motor1, motor2);
        Serial.println("Done turning");
        // delay(2000); // DO NOT INCLUDE IN PRODUCTION
        just_turned = true;
        // is_turning1=false;
        break;
      }
    }
    if (is_left == false)
    {
      inRange(turning_destS - turnBuffer, turning_destS + turnBuffer, distanceS) ? is_turning11 = false : is_turning11 = true;
      inRange(turning_dest - turnBuffer, turning_dest + turnBuffer, distanceN) ? is_turning22 = false : is_turning22 = true;
      // detect_stray_vector(previous_vector, distanceN, false);
      if (is_turning22 == true && is_turning11 == true)
      {
        if (is_left == true)
        {
          left(motor1, motor2, turning_speed);
          Serial.println("Turning left...");
        }
        if (is_left == false)
        {
          right(motor1, motor2, turning_speed);
          Serial.println("Turning right...");
        }
      }
      if (is_turning22 == false)
      {
        brake(motor1, motor2);
        just_turned = true;
        // is_turning22=false;
        break;
      }
    }
  }
  // those two statements let us know the direction we are turning in a unified function. The reason we have an extra function is so we can implement our own algorithms for 90 degree turning.
}

int sos()
{

  soundspeedms = 331.4 + (0.606 * temp) + (0.0124 * hum);

  soundspeedcm = soundspeedms / 10000;
  return soundspeedcm;
}

void find_prox()
{
  /*
  Runs the proximity checks
  we can call this function at anytime to improve the accuracy of our time sitatuion.
  */
  sos();

  durationN = sonarN.ping_median(iterations);
  durationE = sonarE.ping_median(iterations);
  durationW = sonarW.ping_median(iterations);

  // gathering distance
  // we dvide by two so we get only one trip of sound.
  /*

  T ---------\
              \
                >|     (That line is an Object that the sound wave hits. )
              /
  R ---------/
  We only need a set of lines (path of transmit sound) when dealing with how long it tookf or the sound to just GET to the object. To travel through the air .
  Beacuse only one of those sets of lines (1/2) represents the actual distance between the transmitter and the object.
  */
  distanceN = (durationN / 2) * soundspeedcm;
  distanceE = (durationE / 2) * soundspeedcm;
  distanceW = (durationW / 2) * soundspeedcm;
  // end of gatehring distance
}

// void detect_stray_vector(float prev_vector, float new_vector)
// {

//   prevTimevec += timeDelayvec;
//   if (corrected == 2)
//   {
//     corrected = 0;
//     distanceN = new_vector;
//   }
//   if (new_vector - prev_vector < 110 && new_vector - prev_vector > 30 && corrected != 2)
//   {
//     corrected += 1;
//     Serial.println("DetectedStrayVector as ");
//     Serial.print(new_vector);
//     distanceN = prev_vector;
//     Serial.println("\n\nASSERTED DISTANCEn TO ");
//     Serial.print(distanceN);
//   }
// }

void find_N()
{
  // unsigned long timeCurrent = millis();
  /*
  only gathers northern one
  */
  sos();

  durationN = sonarN.ping_median(iterations);
  durationS = sonarS.ping_median(iterations);

  distanceN = (durationN / 2) * soundspeedcm;
  distanceS = (durationS / 2) * soundspeedcm;

  // detect_stray_vector(distanceN, distances);
}

void anti_drive()
{
  /*
  This function whishes to prove the car shouldn't be driving every time it is run.
  */

  if (driving == true)
  {
    if (distanceN >= 400 || distanceN <= 13)
    { // || is like or in python
      Nsafe = false;
      brake(motor1, motor2);
      back(motor1, motor2, 120);
      delay(500);
      brake(motor1, motor2);
      driving = false;
    }
    if (distanceN >= 20)
    {
      Nsafe = true;
    }
  }
}

void analyze_surroundings()
{

  if (distanceN >= 400 || distanceN <= 7)
  { // || is like or in python
    Nsafe = false;
  }
  if (distanceN >= 7)
  {

    Nsafe = true;
  }
  if (distanceE >= 400 || distanceE <= EDynaThreshLow)
  { // || is like or in python
    Esafe = false;
  }
  if (distanceE >= EDynaThreshHigh)
  {
    Esafe = true;
  }
  if (distanceW >= 400 || distanceW <= WDynaThreshLow)
  { // || is like or in python
    Wsafe = false;
  }
  if (distanceW >= WDynaThreshHigh)
  {
    Wsafe = true;
  }
  // Serial.println(Nsafe);
  // Serial.println(Esafe);
  // Serial.println(Wsafe);
}

void log_data()
{
  Serial.println("Sound :");
  Serial.print(soundspeedms);
  Serial.print(" m/s, ");
  Serial.print("Humidity:");
  Serial.print(hum);
  Serial.print("%, Temp:");
  Serial.print(temp);
  Serial.println(" C | Distances: ");
  Serial.print(distanceN);
  Serial.print(" cm NORTH | ");
  Serial.print(distanceE);
  Serial.print(" cm EAST | ");
  Serial.print(distanceW);
  Serial.print(" cm WEST | ");
}

void driving_decision()
{
  if (just_turned == true)
  {
    forward(motor1, motor2, 200);
    // delay could be here.
    if (just_turned == true && distanceE <= 20 && distanceW <= 20)
    { // if we are driving and the sides are in range of walls
      brake(motor1, motor2);
      dynathresh();
    }
    Nsafe = true;
    Wsafe = false; // this might be problematic
    Esafe = false;
    just_turned = false;
  }
  anti_drive();
  if (Nsafe == false)
  {

    if (Wsafe == true && Esafe == false && Nsafe == false)
    {
      // turn left
      brake(motor1, motor2);

      turn(true, true);
    }
    if (Wsafe == false && Esafe == true && Nsafe == false)
    {
      // turn right
      brake(motor1, motor2);

      turn(true, false);
    }

    if (Wsafe == false && Esafe == false && Nsafe == false)
    {
      // dead end
      Serial.println("Dead end"); // not yet coded to handle dead ends. Dead ends may be fired by a miscalculation too.
    }
    if (Wsafe == true && Esafe == true && Nsafe == false)
    {
      if (berserk_mode == true)
      {
        forward(motor1, motor2, 200);
        delay(5000);
      }
    }
  }
  if (Nsafe == true && Wsafe == false && Esafe == false)
  {
    // drive forward

    if (berserk_mode == true)
    {
      forward(motor1, motor2, 200);
      delay(5000);
    }
    forward(motor1, motor2, 125);

    driving = true;
  }

  if (Wsafe == true && Esafe == true && Nsafe == true)
  {
    Serial.println("Driving north in an open field"); // berskerk mode
    if (berserk_mode == true)
    {
      forward(motor1, motor2, 200);
      delay(4000);
    }
    maybe_I_move();
  }
}

void setup()
{
  Serial.begin(115200);
  dht.begin();
  pingTimer = millis();
  pinMode(buttonPin, INPUT);
  buttonState = digitalRead(buttonPin);
  Serial.print("Hold button for access to non maze mode...");
  Serial.print("Delay in Setup");
  delay(2000);
  if (buttonState == HIGH)
  {
    Serial.print("GOING BERSERK");
    berserk_mode = true;
  }
  else
  {
    find_prox();
    dynathresh();
  }
  hum = dht.readHumidity();
  temp = dht.readTemperature();
}

// last-ditch effort when undergoing a stalemate in terms of completing the maze.
void maybe_I_move()
{
  if (distanceN <= 15)
  {
    if (distanceE > distanceW + 3 || distanceE == 0)
    { // turn east
      turn(false, true);
    }
    if (distanceE < distanceW + 3 || distanceW == 0)
    { // turn west
      turn(true, true);
    }
  } // generaly confirming if we see a dead end
}

void loop()
{
  // unsigned long timeCurrent = millis();
  // gather proximities
  unsigned long timeCurrent = millis();
  // unsigned long timeCurrent = millis();
  // gather proximities
  if (timeCurrent - prevTimeprox > timeDelayprox)
  {
    prevTimeprox += timeDelayprox;
    find_prox();
  }

  analyze_surroundings();
  driving_decision();
  log_data();
}
// shut up no test