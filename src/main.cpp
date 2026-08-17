#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "imu.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
const uint8_t WHO_AM_I_REGISTER = 0x75;

unsigned long previous_time;

void scanI2C(){
    Wire.begin(21,22);
    for (int address = 1; address < 127; address++){
        Wire.beginTransmission(address);
        int status = Wire.endTransmission();


        if (status == 0){
            Serial.print("Found device at 0x");
            Serial.println(address, HEX);
    
        }

    }
}

 void setup()
{
    Serial.begin(115200);
    scanI2C();
    pwm.begin();
    pwm.setPWMFreq(50);
    delay(1000);
    previous_time = micros();

//Test MPU
    /* Wire.beginTransmission(0x68);
    Wire.write(WHO_AM_I_REGISTER);
    Wire.endTransmission(false);

    Wire.requestFrom(0x68, 1);

    if (Wire.available()) {
        int whoami = Wire.read();
        Serial.print("WHO_AM_I = 0x");
        Serial.println(whoami, HEX);
    } */


}
void loop()
{

   //Read IMU
   IMUData imu = readIMU();

    
    //calculate dt for integration:
    unsigned long current_time = micros();
    float dt = (current_time - previous_time) / 1000000.0f; //micro s to s 
    previous_time = current_time;

    updateComplementaryFilter(imu, dt);
    
   static unsigned long last_print = 0;

    // to slow down print
    if (millis() - last_print >= 100) {
        Serial.print("Angle X: ");
        Serial.print(imu.theta_x);

        Serial.print(" Angle Y: ");
        Serial.println(imu.theta_y);

        last_print = millis();
    }
}
