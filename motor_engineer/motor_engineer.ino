#include <SparkFun_TB6612.h>


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

void setup()
{
 
}


void loop()
{
   //Use of the drive function which takes as arguements the speed
   //and optional duration.  A negative speed will cause it to go
   //backwards.  Speed can be from -255 to 255.  Also use of the 
   //brake function which takes no arguements.
    left(motor1, motor2, 255);
    delay(10000);
    right(motor1, motor2, 255);

    

   
  //  //Use of the drive function which takes as arguements the speed
  //  //and optional duration.  A negative speed will cause it to go
  //  //backwards.  Speed can be from -255 to 255.  Also use of the 
  //  //brake function which takes no arguements.
  //  motor2.drive(255,1000);
  //  motor2.drive(-255,1000);
  //  motor2.brake();
  //  delay(1000);
   
  //  //Use of the forward function, which takes as arguements two motors
  //  //and optionally a speed.  If a negative number is used for speed
  //  //it will go backwards
  //  forward(motor1, motor2, 150);
  //  delay(1000);
   
  //  //Use of the back function, which takes as arguments two motors 
  //  //and optionally a speed.  Either a positive number or a negative
  //  //number for speed will cause it to go backwards
  //  back(motor1, motor2, -150);
  //  delay(1000);
   
  //  //Use of the brake function which takes as arguments two motors.
  //  //Note that functions do not stop motors on their own.
  //  brake(motor1, motor2);
  //  delay(1000);
   
  //  //Use of the left and right functions which take as arguements two
  //  //motors and a speed.  This function turns both motors to move in 
  //  //the appropriate direction.  For turning a single motor use drive.

  //  //Use of brake again.
  //  brake(motor1, motor2);
  //  delay(1000);
   
}