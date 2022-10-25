class BAR
{
public:
    BAR();
    ~BAR();
    void ConvertBARData(int x);
    void Calibrate_BAR(int x)
    int getHeight() const;
private:
    int initial_AMSL_;
    int initial_pressure_;
    int height_AMSL_;
    int height_AGL_;
    uint32_t pressure_; // Temperature compensated pressure in converted from mbar to bar and then to KPa.
};