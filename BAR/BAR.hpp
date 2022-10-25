class BAR
{
public:
    BAR();
    ~BAR();
    void ConvertBARData(int x);
    int getHeight();
private:
    int height_AMSL_;
    int height_AGL_;
    int AGL_AMSL_diff_;
    uint32_t pressure_; // Temperature compensated pressure in converted from mbar to bar and then to KPa.
};