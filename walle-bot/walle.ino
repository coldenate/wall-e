
// ITS WALLE

#define triggerPin 10
#define echoPin 13

float duration, distance, sos;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void loop()
{
    // put your main code here, to run repeatedly:
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    sos = 0.0343;                    // speed of sound
    distance = (duration / 2) * sos; // divided by two to get return wave

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
