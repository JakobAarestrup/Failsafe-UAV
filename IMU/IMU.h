class IMU
{
    public:
    IMU();
    void initIMU();
    void closeIMU();
    void IMU_Calc();
    private:
    float IMU_Data;
};