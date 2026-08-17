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

    float theta_x;
    float theta_y;
};

IMUData readIMU();
void updateComplementaryFilter(IMUData& data, float dt);