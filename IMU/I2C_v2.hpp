class I2C
{
public:
    I2C(); // Constructor
    ~I2C(); // Destructor
    void InitializeI2C();
    void WriteI2C(int ADDR, int reg, int length, int data); // Write to I2C device register
    int ReadI2C(int ADDR, int reg, int length, float recipient); // Read from I2C device register
private:
    float I2CData_; // I2C device data
    float Buffer1_;
    float Buffer2_;
};