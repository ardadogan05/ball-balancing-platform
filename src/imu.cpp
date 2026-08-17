#include "imu.h"
#include <Wire.h>

const uint8_t MPU_ADDR = 0x68;
const uint8_t ACCEL_XOUT_H = 0x3B;


static bool filter_initialized = false;
static float theta_x = 0.0;
static float theta_y = 0.0;

IMUData readIMU(){
    IMUData data; 

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
    data.ax_g = ax / 16384.0f;
    data.ay_g = ay / 16384.0f;
    data.az_g = az / 16384.0f;

    //angle of rotation about x and y axis
    data.angle_x = atan2(ay, az) * 180.0f / PI;
    data.angle_y = atan2(-ax, az)* 180.0f / PI;

    //raw gyro values
    int16_t gx = (gx_high << 8) | gx_low;
    int16_t gy = (gy_high << 8) | gy_low;
    int16_t gz = (gz_high << 8) | gz_low;

    //converted into dps, adjusted for bias from 233 values
    data.gx_dps = (gx - 424) / 131.0f;
    data.gy_dps = (gy - 134) / 131.0f;
    data.gz_dps = (gz - 187) / 131.0f;  

    return data;
}

void updateComplementaryFilter(IMUData& data, float dt){
    //for weighted average
    const float tau = 0.5f;
    float alpha = tau / (tau + dt);

    if (!filter_initialized) {
        theta_x = data.angle_x;
        theta_y = data.angle_y;
        filter_initialized = true;
    }
    //integrate gyro to get angle from angular velocity

    theta_x = alpha*(theta_x + data.gx_dps * dt) + (1-alpha) * data.angle_x;
    theta_y = alpha*(theta_y + data.gy_dps * dt) + (1-alpha) * data.angle_y;

    data.theta_x = theta_x;
    data.theta_y = theta_y;

}