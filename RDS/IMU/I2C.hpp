#pragma once
class I2C
{
public:
    I2C();
    ~I2C();
    void initializeI2C();
    void writeI2C(int ADDR, int reg, int length, unsigned char *data);
    float readI2C(int ADDR, int reg, int length, int HandleI2C);

private:
    /// @brief Read I2C data is set in this private variable
    float dataI2C_;
};