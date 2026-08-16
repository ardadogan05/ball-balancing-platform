#pragma once
#include <Arduino.h>

struct IMUData {
    float ax_g;
    float ay_g;
    float az_g;

    float gx_dps;
    float gy_dps;
    float gz_dps;

    float angle_x;
    float angle_y;
};

void initIMU();
IMUData readIMU();