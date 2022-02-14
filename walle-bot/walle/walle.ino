
// ITS WALLE

#include "DHT.h"
#include "NewPing.h"

#define triggerPin 10 //yellow
#define echoPin 13 // orange
#define MAX_DISTANCE 400 

#define DHTPIN 7
#define DHTYPE DHT11


NewPing sonar(triggerPin, echoPin, MAX_DISTANCE);

float duration;
float hum;
float temp;
float distance;
float soundspeedms;
float soundspeedcm;

int iterations = 6;

DHT dht(DHTPIN, DHTYPE);

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    dht.begin();
}

void loop()
{
    delay(2000);

    hum = dht.readHumidity();
    temp = dht.readTemperature();

    soundspeedms = 331.4 + (0.606 *temp) +(0.0124 * hum);

    soundspeedcm = soundspeedms / 10000;
    
    duration = sonar.ping_median(iterations);

    distance = (duration/2)*soundspeedcm;

    Serial.print("Sound :");
    Serial.print(soundspeedms);
    Serial.print(" m/s, ");
    Serial.print("Humidity:");
    Serial.print(hum);
    Serial.print("%, Temp:");
    Serial.print(hum);
    Serial.print(" C | Distance:");
    
    
    
 
    Serial.print("Distance is equal to ");
    if (distance >= 400 || distance <= 2)
    {
        Serial.println("Out of range :(");
    }

    else
    {
        
        Serial.print(distance);
        Serial.println(" cm");
        delay(500);
    }
    delay(500);
}
