#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define PI 3.14159265358979323846
#define AA 0.97
#define DT 0.02

void complementaryFilter(float xl_Roll, float xl_Pitch, float magYaw, float gy_Roll, float gy_Pitch, float gy_Yaw)
{
    printf("xl_Roll: %f xl_Pitch: %f\n", xl_Roll, xl_Pitch);
    printf("Yaw: %f\n", magYaw);
    /*Complementary Filter*/
    double compRoll, compPitch, compYaw;

    compRoll = AA * (compRoll + gy_Roll) + (1 - AA) * xl_Roll;     // 97% Gyro 3% Accelerometer
    compPitch = AA * (compPitch + gy_Pitch) + (1 - AA) * xl_Pitch; // 97% Gyro 3% Accelerometer
    compYaw = 0.5 * (compYaw + gy_Yaw) + (1 - 0.5) * magYaw;       // 50% Gyro 50% Magnometer

    printf("Roll filtered: %f, Pitch filtered: %f, Yaw Filtered: %f\n", compRoll, compPitch, compYaw);
}

int main()
{
    float xl_Roll, xl_Pitch;
    float gyro_Roll, gyro_Pitch, gyro_Yaw;
    float magYaw;
    xl_Roll = 30;
    xl_Pitch = 0;
    magYaw = 90;
    gyro_Roll = 20;
    gyro_Pitch = 2;
    gyro_Yaw = 80;
    complementaryFilter(xl_Roll, xl_Pitch, magYaw, gyro_Roll, gyro_Pitch, gyro_Yaw);
    xl_Roll = 100;
    xl_Pitch = 19;
    magYaw = 300;
    gyro_Roll = 40;
    gyro_Pitch = 20;
    gyro_Yaw = 80;
    complementaryFilter(xl_Roll, xl_Pitch, magYaw, gyro_Roll, gyro_Pitch, gyro_Yaw);

    return 0;
}
