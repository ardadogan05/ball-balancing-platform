#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
const uint8_t WHO_AM_I_REGISTER = 0x75;
const uint8_t MPU_ADDR = 0x68;
const uint8_t ACCEL_XOUT_H = 0x3B;

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
    delay(1000);
    Serial.println("ESP32 started successfully");

    scanI2C();

    pwm.begin();
    pwm.setPWMFreq(50);
    delay(1000);
    
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
    //Read MPU: 
    delay(400); //for readability
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(ACCEL_XOUT_H);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 14);

    uint8_t ax_high = Wire.read();
    uint8_t ax_low  = Wire.read();

    uint8_t ay_high = Wire.read();
    uint8_t ay_low  = Wire.read();

    uint8_t az_high = Wire.read();
    uint8_t az_low  = Wire.read();

    Wire.read(); //just reading and ignoring temp values
    Wire.read();

    uint8_t gx_high = Wire.read();
    uint8_t gx_low  = Wire.read();

    uint8_t gy_high = Wire.read();
    uint8_t gy_low  = Wire.read();

    uint8_t gz_high = Wire.read();
    uint8_t gz_low  = Wire.read();

    //raw accelerometer values:
    int16_t ax = (ax_high << 8) | ax_low;
    int16_t ay = (ay_high << 8) | ay_low;
    int16_t az = (az_high << 8) | az_low;

    //converting to units of g 
    float ax_g = ax / 16384.0f;
    float ay_g = ay / 16384.0f;
    float az_g = az / 16384.0f;

    //raw gyro values
    int16_t gx = (gx_high << 8) | gx_low;
    int16_t gy = (gy_high << 8) | gy_low;
    int16_t gz = (gz_high << 8) | gz_low;

    //converted into dps, adjusted for bias from 233 values
    float gx_dps = (gx - 424) / 131.0f;
    float gy_dps = (gy - 134) / 131.0f;
    float gz_dps = (gz - 187) / 131.0f;

    Serial.print("AX: ");
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
    Serial.println(" °/s  ");
}
