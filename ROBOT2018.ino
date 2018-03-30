
/****************************************************
 * Authors: Mitch, Jose, Baldemar, Nicholas, Chris  *
 * Description: IEEE Robotics Code                  *
 * Date: 3/21/2018                                  *
 ***************************************************/
#include <Wire.h>
#include<String.h>
#include <math.h>
#include "functions.h"

#define uchar unsigned char

uchar t;
//void send_data(short a1,short b1,short c1,short d1,short e1,short f1);
uchar data[16];

int roundnumber = 1;
int Tokens = 12 + (6 * roundnumber) - 6;

// Motor A, right wheel
int enA = 10;  //cable green 
int in1 = 9;  //cable blue
int in2 = 8;  //cable purple

// Motor B, left wheel
int enB = 5;  //cable black 
int in3 = 7;  //cable grey
int in4 = 6;  //cable white

// Constants for Interrupt Pins
const byte MOTOR_A = 3;  // Motor 2 Interrupt Pin - INT 1 - Right Motor
const byte MOTOR_B = 2;  // Motor 1 Interrupt Pin - INT 0 - Left Motor
const float stepcount = 20.00;  // 20 Slots in disk, change if different
//const float wheeldiameter = 6.3; // Wheel diameter in millimeters, change if different
const float wheeldiameter = 2.705; // Wheel diameter in millimeters, change if different

// Integers for pulse counters
volatile int counter_A = 0;
volatile int counter_B = 0;

bool startMode = true;
bool turnTrigger = false;
bool interpretMode = false;

int perpCount = 0;


// Interrupt Service Routines  
// Motor A pulse count ISR
void ISR_countA() {
    counter_A++;  // increment Motor A counter value
}

// Motor B pulse count ISR
void ISR_countB() {
    counter_B++;  // increment Motor B counter value
}


//*******************************************
//   setup(): initialization routines       *
//                                          *
//*******************************************
void setup() {
    // Attach the Interrupts to their ISR's
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(MOTOR_A), ISR_countA,
                    RISING);  // Increase counter A when speed sensor pin goes High
    attachInterrupt(digitalPinToInterrupt(MOTOR_B), ISR_countB,
                    RISING);  // Increase counter B when speed sensor pin goes High

    // Test Motor Movement  - Experiment with your own sequences here  
    Wire.begin();        // join i2c bus (address optional for master)
    Serial.begin(9600);  // start serial for output
    t = 0;


}

//FIND 
String findBlacks(uchar *arr, int *sensors) {
    int count = 0;
    int m = 30;
    int ind[8];
    String s = "";
    for (int i = 0; i < 15; i += 2) {
        int a = arr[i];
        if (a < m) {
            s = s + " " + String(i);
            sensors[count] = i;
            count++;
        }
    }
    if (s.length() == 0) {
        s = "-1";
        interpretMode = true;
    } else {
        interpretMode = false;
    }
    return s;
}


bool calcFwd() {
    return isActive(6) || isActive(8) || isActive(10) || isActive(4);
}

// Function to Move Forward
void move(int steps, float mspeed) {
    counter_A = 0;  //  reset counter A to zero
    counter_B = 0;  //  reset counter B to zero

    int rms = 0;
    int lms = 0;

    // Set Motor A forward
    //RIGHT MOTOR
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B forward
    //LEFT MOTOR
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    // Go forward until step value is reached
    bool turned = true;
    while (steps > counter_A && steps > counter_B && turned) {

        bool fwd = calcFwd();
        if (fwd) {
            lms = mspeed;
            rms = mspeed;
            // Set Motor A and B forward
        }

        /*//correct itself moving  right
        if ((isActive(0) || isActive(2)) && !fwd) {
            turn(34);
            fwd = calcFwd();
            turned = false;
            delay(1000);
        }
        if ((isActive(12) || isActive(14)) && !fwd) {
            turn(-34);
            fwd = calcFwd();
            turned = false;
            delay(1000);
        }
       */

        if (steps > counter_A) {
            analogWrite(enA, rms);
        } else {
            analogWrite(enA, 0);
        }
        if (steps > counter_B) {
            analogWrite(enB, lms);
        } else {
            analogWrite(enB, 0);
        }
    }

    // Stop when done
    analogWrite(enA, 0);
    analogWrite(enB, 0);
    counter_A = 0;  //  reset counter A to zero
    counter_B = 0;  //  reset counter B to zero 
}

void checkPerp() {
    bool a = true;
    for (int i = 0; i < 8; i++) {
        if (sensors[i] == -1) {
            a = false;
        }
    }
    if (a)
        perpCount++;
}


void turn(float theta) {
    counter_A = 0;  //  reset counter A to zero
    counter_B = 0;  //  reset counter B to zero

    int mspeed = 255;

    float scale = abs((theta / 360) * (200));
    int steps = (int) scale;

    int rms = mspeed;
    int lms = mspeed;

    if (theta > 0) {
        //turn right
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);

        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
    } else {
        //turn left
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);

        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
    }
    bool turning=false;
    // Go forward until step value is reached
    while (steps > counter_A && steps > counter_B && !turning) {
        turning= turnTriggerSet();
       
        if (steps > counter_A) {
            analogWrite(enA, rms);
        } else {
            analogWrite(enA, 0);
        }
        if (steps > counter_B) {
            analogWrite(enB, lms);
        } else {
            analogWrite(enB, 0);
        }
    }
    
    // Stop when done
    analogWrite(enA, 0);
    analogWrite(enB, 0);
    counter_A = 0;  //  reset counter A to zero
    counter_B = 0;  //  reset counter B to zero 
}

float turnLen(float theta) {
    float cw = 40.8407;
    return abs((theta / 360) * cw);
}

void square(bool left) {
    if (turnTrigger) {
        turn(-90);

    } else {
        move(20, 255);
    }
}

bool turnTriggerSet() {
    bool a =  isActive(10) && isActive(12) && isActive(14);
    bool b = isActive(0) && isActive(2) && isActive(4);
    return turnTrigger = a || b;
}

void loop() {
    for (int i = 0; i < 8; i++) {
        sensors[i] = -1;
    }
    Wire.requestFrom(9, 16);    // request 16 bytes from slave device #9
    while (Wire.available()) // slave may send less than requested
    {
        data[t] = Wire.read(); // receive a byte as character
        if (t < 15)
            t++;
        else
            t = 0;
    }
    String bl = findBlacks(data, sensors);
    Serial.print(bl);
    Serial.print(" ");
    //Serial.print(isLine());
    Serial.println("");
    checkPerp();
    square(true);
}
