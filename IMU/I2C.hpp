

class I2C
{
public:
    I2C();
    void openI2C(int ADDR);
    void WriteI2C(int reg, int data);
    int ReadI2C(int ADDR, int reg);
    double ConvertI2CData(int x, int y, int z);
    ~I2C();
private:
    float I2C_Data_;
    int I2C_File_;
    int Write_File_;
};