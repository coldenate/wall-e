
// ITS WALLE

#include "DHT.h"
#include "NewPing.h"


#define DHTPIN 3
// West
#define trigPin_W 8
#define echoPin_W 9
// East
#define trigPin_E 10
#define echoPin_E 11
// North (all in terms of driving forward)
#define triggerPin 12
#define echoPin 13

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

DHT dht(DHTPIN, DHTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
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

  Serial.println("Sound :");
  Serial.print(soundspeedms);
  Serial.print(" m/s, ");
  Serial.print("Humidity:");
  Serial.print(hum);
  Serial.print("%, Temp:");
  Serial.print(temp);
  Serial.println(" C | Distances: ");



  Serial.print("\t\t\t\t\t\t\t\t");
  // tht was a bunch f tbats????
  Serial.print("N:");
  if (distance >= 400 || distance <= 2) {
    Serial.print("Out of range | ");
  }

  else {

    Serial.print(distance);
    Serial.print(" cm | ");
  }
  Serial.print("E:");
  if (distanceE >= 400 || distanceE <= 2) {
    Serial.print("Out of range | ");
  }

  else {

    Serial.print(distanceE);
    Serial.print(" cm | ");
  }
  Serial.print("W:");
  if (distanceW >= 400 || distanceW <= 2) {
    Serial.println("Out of range | ");
  }

  else {

    Serial.print(distanceW);
    Serial.print(" cm | ");
  }
}
