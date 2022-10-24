class I2C
{
public:
    I2C();
    void openI2C(int ADDR);
    void WriteI2C(int ADDR, int reg, int data);
    int ReadI2C(int ADDR, int reg);
    ~I2C();
private:
    unsigned int I2C_Data_;
    int I2C_File_;
    int Write_File_;
};