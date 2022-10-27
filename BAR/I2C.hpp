class I2C
{
public:
    I2C(); // Constructor
    ~I2C(); // Destructor
    void openI2C(int ADDR); // Open i2c communication
    void WriteI2C(int ADDR, int reg, int data); // Write to I2C device register
    int ReadI2C_16bit(int ADDR, int reg); // Read from I2C device register
    int ReadI2C_8bit(int ADDR, int reg); // Read from I2C device register
private:
    int I2C_Data_; // I2C device data
    int I2C_File_; // I2C communication file
    int Write_File_; // I2C Write file
    int Reg_data_;
};