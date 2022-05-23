#define buttonPin 4

int buttonState = 0; // a value for the default (OFF) button state | this represents the button NOT completing a circuit flow.


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
  pinMode(buttonPin, INPUT);
  buttonState = digitalRead(buttonPin);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (buttonState == HIGH) {
    Serial.print("GOING BERSERK");}
}
