
// ITS WALLE

#include "DHT.h"
#include "NewPing.h"


#define DHTPIN 0
// West
#define trigPin_W 1
#define echoPin_W 2
// East
#define trigPin_E 3
#define echoPin_E 4
// North (all in terms of driving forward)
#define triggerPin 5
#define echoPin 6

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
bool Nsafe;
bool Esafe; 
bool Wsafe;

DHT dht(DHTPIN, DHTYPE);

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




  hum = dht.readHumidity();
  temp = dht.readTemperature();

  soundspeedms = 331.4 + (0.606 * temp) + (0.0124 * hum);

  soundspeedcm = soundspeedms / 10000;

  // end of temperature sensor data collection

  duration = sonar.ping_median(iterations);
  durationE = sonarE.ping_median(iterations);
  durationW = sonarW.ping_median(iterations);

  distance = (duration / 2) * soundspeedcm;
  distanceE = (durationE / 2) * soundspeedcm;
  distanceW = (durationW / 2) * soundspeedcm;
  if (debug_mode == true) {

    // Serial.println("Sound :");
    // Serial.print(soundspeedms);
    // Serial.print(" m/s, ");
    // Serial.print("Humidity:");
    // Serial.print(hum);
    // Serial.print("%, Temp:");
    // Serial.print(temp);
    // Serial.println(" C | Distances: ");
  }


  Serial.println();
  Serial.print("\t\t\t\t\t\t\t\t");
  // tht was a bunch f tbats????
  if (distance >= 400 || distance <= 5) {
    Nsafe = false;
    if (debug_mode == true) {
    Serial.print("Out of range | ");}
  }

  else {
    Nsafe=true;
    if (debug_mode == true) {
    Serial.print(distance);
    Serial.print(" cm | ");}
  }

  if (distanceE >= 400 || distanceE <= 5) {
    Esafe = false;
    if (debug_mode == true) {
    Serial.print("Out of range | ");
  }}

  else {
    Esafe=true;
    if (debug_mode == true) {
    Serial.print(distanceE);
    Serial.print(" cm | ");}
  }
  if (distanceW >= 400 || distanceW <=5) {
    Wsafe=false;
    if (debug_mode == true) {
    Serial.println("Out of range | ");
  }}

  else {
    Wsafe=true;
    if (debug_mode == true) {
    Serial.print(distanceW);
    Serial.print(" cm | ");}
  }
  if (Nsafe == true) {
    stdf=true;
    Serial.println("I am driving forward");
  }
  if (Nsafe==false) {
    Serial.println("Wait crap there is dead end man cmon. Traffic jammmm");
    stdf=false;
    // at this point, the decision tree will decide if we should turn left or right
    if (Wsafe == true) {
      // turn left
      Serial.print("Turn leftt");
    }
    if (Esafe == true) {
      // turn right
      Serial.print("Turn right");
    }
  }

  
}
