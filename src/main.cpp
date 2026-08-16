#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "imu.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
const uint8_t WHO_AM_I_REGISTER = 0x75;

unsigned long previous_time;
float gyro_angle_x = 0;
float gyro_angle_y = 0;

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
    initIMU();
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
    delay(400); //for readability
   //Read IMU
   IMUData imu = readIMU();

    
    //calculate dt for integration:
    unsigned long current_time = micros();
    float dt = (current_time - previous_time) / 1000000.0f; //micro s to s 
    previous_time = current_time;

    //integrate gyro to get angle from angular velocity

    gyro_angle_x += imu.gx_dps * dt;
    gyro_angle_y += imu.gy_dps * dt; 


    /* Serial.print("AX: ");
    Serial.print(ax_g);
    Serial.print(" g  ");

    Serial.print(" AY: ");
    Serial.print(ay_g);
    Serial.print(" g  ");

    Serial.print(" AZ: ");
    Serial.print(az_g);
    Serial.println(" g  ");

    Serial.print("GX: ");
    Serial.print(gx_dps);
    Serial.print(" °/s  ");

    Serial.print(" GY: ");
    Serial.print(gy_dps);
    Serial.print(" °/s  ");

    Serial.print(" GZ: ");
    Serial.print(gz_dps);
    Serial.println(" °/s  "); */

    Serial.print("Angle X: ");
    Serial.print(imu.angle_x);
    Serial.print(" deg   ");

    Serial.print("Angle Y: ");
    Serial.print(imu.angle_y);
    Serial.println(" deg");

    Serial.print("Gyro x: ");
    Serial.print(gyro_angle_x);
    Serial.print(" deg   ");

    Serial.print("Gyro Y: ");
    Serial.print(gyro_angle_y);
    Serial.println(" deg");
}
