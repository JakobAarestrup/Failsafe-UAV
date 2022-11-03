class Kalman
{
public:
    Kalman();
    ~Kalman();
    float Kalman::KalmanfilterX(float accAngle, float gyroRate);
    float Kalman::KalmanfilterY(float accAngle, float gyroRate);
    float Kalman::KalmanfilterZ(float gyrAngle, float heading);
    int Kalman::mymillis();
    int Kalman::timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
private:
float Q_angle  =  0.01;
float Q_gyro   =  0.0003;
float R_angle  =  0.01;
float x_bias = 0;
float y_bias = 0;
float z_bias = 0;
float XP_00 = 0, XP_01 = 0, XP_10 = 0, XP_11 = 0;
float YP_00 = 0, YP_01 = 0, YP_10 = 0, YP_11 = 0;
float ZP_00 = 0, ZP_01 = 0, ZP_10 = 0, ZP_11 = 0;
float KFangleX = 0.0;
float KFangleY = 0.0;
float KFangleZ = 0.0;
}

#define DT 0.02         // [s/loop] loop period. 20ms

float Kalman::KalmanfilterX(float accAngle, float gyroRate)
{
    float  x, S;
    float K_0, K_1;
    KFangleX += DT * (gyroRate - x_bias);
    XP_00 +=  - DT * (XP_10 + XP_01) + Q_angle * DT;
    XP_01 +=  - DT * XP_11;
    XP_10 +=  - DT * XP_11;
    XP_11 +=  + Q_gyro * DT;
    x = accAngle - KFangleX;
    S = XP_00 + R_angle;
    K_0 = XP_00 / S;
    K_1 = XP_10 / S;
    KFangleX +=  K_0 * x;
    x_bias  +=  K_1 * x;
    XP_00 -= K_0 * XP_00;
    XP_01 -= K_0 * XP_01;
    XP_10 -= K_1 * XP_00;
    XP_11 -= K_1 * XP_01;
    return KFangleX;
}

float Kalman::KalmanfilterY(float accAngle, float gyroRate)
{
    float  y, S;
    float K_0, K_1;
    KFangleY += DT * (gyroRate - y_bias);
    YP_00 +=  - DT * (YP_10 + YP_01) + Q_angle * DT;
    YP_01 +=  - DT * YP_11;
    YP_10 +=  - DT * YP_11;
    YP_11 +=  + Q_gyro * DT;
    y = accAngle - KFangleY;
    S = YP_00 + R_angle;
    K_0 = YP_00 / S;
    K_1 = YP_10 / S;
    KFangleY +=  K_0 * y;
    y_bias  +=  K_1 * y;
    YP_00 -= K_0 * YP_00;
    YP_01 -= K_0 * YP_01;
    YP_10 -= K_1 * YP_00;
    YP_11 -= K_1 * YP_01;
    return KFangleY;
}

float Kalman::KalmanfilterZ(float gyroRate, float heading)
{
    float  z, S;
    float K_0, K_1;
    KFangleZ += DT * (gyroRate - z_bias);
    ZP_00 +=  - DT * (ZP_10 + ZP_01) + Q_angle * DT;
    ZP_01 +=  - DT * ZP_11;
    ZP_10 +=  - DT * ZP_11;
    ZP_11 +=  + Q_gyro * DT;
    z = heading - KFangleZ;
    S = ZP_00 + R_angle;
    K_0 = ZP_00 / S;
    K_1 = ZP_10 / S;
    KFangleZ +=  K_0 * z;
    z_bias  +=  K_1 * z;
    ZP_00 -= K_0 * ZP_00;
    ZP_01 -= K_0 * ZP_01;
    ZP_10 -= K_1 * ZP_00;
    ZP_11 -= K_1 * ZP_01;
    return KFangleZ;
}

int Kalman::mymillis()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec) * 1000 + (tv.tv_usec)/1000;
}

int Kalman::timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;
    return (diff<0);
}