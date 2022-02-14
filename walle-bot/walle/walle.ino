
// ITS WALLE

#include "NewPing.h"

#define triggerPin 10
#define echoPin 13
#define MAX_DISTANCE 400

NewPing sonar(triggerPin, echoPin, MAX_DISTANCE);

float duration, distance, sos;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop()
{
    duration = sonar.ping();

    distance = (duration/2)*0.0343;
    
 
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
