#include "ValidateStateV1.hpp"

/**
 * @brief Construct a Validate State object
 *
 */
ValidateState::ValidateState()
{
    // state_ = 0; // Normal State
}

/**
 * @brief Destroy the Validate State object
 *
 */
ValidateState::~ValidateState()
{
}

/**
 * @brief Configuration arguments for RDS.exe
 *
 */
void ValidateState::Usage(const std::string &bin_name)
{
    std::cerr << "Usage : " << bin_name << " <serial> <max_height> <max_acceleration> <max_orientation> <max_distance>\n\n"
              << "Serial format should be :\n"
              << " Serial : serial:///path/to/serial/dev[:baudrate]\n"
              << "\n"
              << "Critical Values format should be :\n"
              << " - Max Height       : 0-300 m\n"
              << " - Max Acceleration : 0-25 m/s^2\n"
              << " - Max Orientation  : 0-45 degrees\n"
              << " - Max distance from border : 0-100 m\n"
              << " - Enter Default for : 300 m 15 m/s^2 45 degrees 50 m\n"
              << " - Enter 0s for default values.."
              << " - Example for Serial and Critical Values : serial:///dev/ttyS0:57600 200 10 30 30\n";
}
/**
 * @brief configures the state of the Validate State
 *
 */
void ValidateState::configValidateState(char maxHeight[], char maxAcceleration[], char maxOrientation[], char maxDistance[])
{
    if ((strcmp(maxHeight, "Default") == 0))
    {
        return;
    }

    else if (maxHeight != NULL)
    {
        if (atof(maxHeight) != 0)
        {
            maxHeight_ = atof(maxHeight);
        }
    }

    if (maxAcceleration != NULL)
    {
        if (atof(maxAcceleration) != 0)
        {
            maxAcceleration_ = atof(maxAcceleration);
        }
    }

    if (maxOrientation != NULL)
    {
        if (atof(maxOrientation) != 0)
        {
            maxOrientation_ = atof(maxOrientation);
        }
    }

    if (maxDistance != NULL)
    {
        if (atof(maxDistance) != 0)
        {
            maxDistance_ = atof(maxDistance);
        }
    }

    std::cout << "\n"
              << maxHeight_ << "\n";
    std::cout << maxAcceleration_ << "\n";
    std::cout << maxOrientation_ << "\n";
    std::cout << maxDistance_ << "\n";
}

/**
 * @brief Get the Values from MAVLink
 *
 */
void ValidateState::SetMAVLinkValues(float alt, float lng, float lat, float roll, float pitch, float yaw)
{
    altitudeSYS_ = alt;
    longitudeSYS_ = lng;
    latitudeSYS_ = lat;
    RollSYS_ = roll;
    PitchSYS_ = pitch;
    YawSYS_ = yaw;
}

/**
 * @brief Gets the available GPS data from the NEO7 sensor and asks the drone over mavlink for their data.
 *
 * @param NEO Class object of GPS class
 */
void ValidateState::GetGPSValues(GPS NEO)
{
    NEO.readGPS();                      // reads NMEA message
    NEO.convertData();                  // converts to decimal degrees format
    longitudeRDS_ = NEO.getLongitude(); // returns longitude
    latitudeRDS_ = NEO.getLatitude();   // returns latitude
    NEO.getNorthSouth(longPoleRDS_);    // returns either a north pole or south pole
    NEO.getEastWest(latPoleRDS_);       // returns either a East pole or West pole
    SatellitesRDS_ = NEO.getSV();
}

/**
 * @brief Gets Roll, Pitch and YAW from IMU object and over MAVLINK from Drone
 *
 * @param sensor Class object of IMU class
 */
void ValidateState::GetIMUValues(IMU &sensor_one) //, IMU &sensor_two) IMU void ValidateState::GetIMUValues(IMU sensor)
{
    float roll, pitch, yaw;
    sensor_one.readIMU(2);
    sensor_one.ConvertACCData();
    sensor_one.ConvertMagData();
    sensor_one.ComplementaryFilter();

    roll = sensor_one.getRoll();
    pitch = sensor_one.getPitch();
    yaw = sensor_one.getYaw();

    /* sensor_two.readIMU(2);
    sensor_two.ConvertACCData();
    sensor_two.ConvertMagData();
    sensor_two.ComplementaryFilter();

    roll = roll + sensor_two.getRoll();
    pitch = pitch + sensor_two.getPitch();
    yaw = yaw + sensor_two.getYaw(); */

    {
        std::scoped_lock<std::mutex> lock(m);
        RollRDS_ = roll;
        PitchRDS_ = pitch;
        YawRDS_ = yaw;
    }
}

/**
 * @brief Gets all barometer values needed to get height
 *
 * @param barometer Class object of BAR class
 */
void ValidateState::GetBaroValues(BAR barometer)
{
    barometer.readPressure();
    barometer.readTemperature();
    barometer.calculatePressureAndTemperature();
    altitudeRDS_ = barometer.getHeight();
}

/**
 * @brief Updates all private variables in ValidateState for critical failure status.
 *
 * @param NEO Class object of GPS class
 * @param barometer  Class object of BAR class
 * @param sensor Class object of IMU class
 */
void ValidateState::UpdateSystemValues(BAR barometer) //, IMU sensor)
{
    // GetGPSValues(NEO);
    GetBaroValues(barometer);
    // GetIMUValues(sensor);
}

/**
 * @brief logs all read data from the available sensors.
 *
 */
void ValidateState::LogData(UDP Client) // &Client)
{
    {
        std::scoped_lock<std::mutex> lock(m);
        StateRoll_ = RollRDS_;
        StatePitch_ = PitchRDS_;
        StateYaw_ = YawRDS_;
    }

    /*START logging*/
    printf("Logging data called..\n");
    /*RDS sensors*/
    std::string GPSBaro = "Longitude: " + std::to_string(longitudeRDS_) + " " + longPoleRDS_[0] + " Latitude: " + std::to_string(latitudeRDS_) + " " + latPoleRDS_ + " Satellites: " + std::to_string(SatellitesRDS_) + " Altitude: " + std::to_string(altitudeRDS_);
    Logger(GPSBaro);

    std::string IMU = "Roll: " + std::to_string(StateRoll_) + " Pitch: " + std::to_string(StatePitch_) + " Yaw: " + std::to_string(StateYaw_);
    Logger(IMU);

    std::string GPSBaroSYS = "LongitudeSYS: " + std::to_string(longitudeSYS_) + " " + longPoleRDS_[0] + " LatitudeSYS: " + std::to_string(latitudeSYS_) + " " + latPoleRDS_ + " AltitudeSYS: " + std::to_string(altitudeSYS_);
    Logger(GPSBaroSYS);
    std::string IMUSYS = "RollSYS: " + std::to_string(RollSYS_) + " PitchSYS: " + std::to_string(PitchSYS_) + " YawSYS: " + std::to_string(YawSYS_);
    Logger(IMUSYS);

    // UDP SEND PART
    // char receivedServerMSG[1024];

    std::string RDSData = GPSBaro + IMU;
    std::string SYSData = GPSBaroSYS + IMUSYS;

    const char *RDS = RDSData.c_str();
    const char *SYS = SYSData.c_str();

    Client.UDP_COM(RDS);
    Client.UDP_COM(SYS);
}

/**
 * @brief Checks for critical failure for orientation
 *
 */
int ValidateState::AxisControl(int critical)
{
    float errorOrientation = (maxOrientation_ * 0.66666666);

    // Handler til hvis det ikke er et tal?
    /* StateRoll_ = (StateRoll_ + RollSYS_) / 2;
    StatePitch_ = (StatePitch_ + PitchSYS_) / 2; */

    if (state_ == 1)
        printf("Error State\n");
    else
        printf("Normal State \n");

    if ((StateRoll_ > 45) | (StateRoll_ > 45))
    {
        landDrone();
        critical = 1;
    }
    else if (((StateRoll_ < errorOrientation) & (state_ == 1)) | ((StatePitch_ < errorOrientation) & (state_ == 1)))
    {
        state_ = 0;
        printf("Changing state... to Normal\n");
    }
    else if ((StateRoll_ > errorOrientation) | (StatePitch_ > errorOrientation))
    {
        state_ = 1;
        printf("Changing state... to Error_State Axis\n");
    }
    return critical;
}

/**
 * @brief Checks if the drone is following the correct path
 *
 */
int ValidateState::RouteControl(int critical)
{
    /* if (state_ == 1)
        printf("Error_State\n");
    else
        printf("Normal State \n"); */

    /*   if(altitudeSYS_ > 300 | altitudeRDS_ > 300)
      {
          landDrone();
          critical = 1;
      }

      else if (altitudeSYS_ > 200 | altitudeRDS_ > 200)
      {
          state_ = 1;
          printf("Closing in on ERROR!!! Changing state... to Error_State\n");
      }

      else if (altitudeSYS_ < 200 | altitudeRDS_ < 200)
      {
          state_ = 0;
          printf("Changing state... to Normal\n");
      }
      return critical;
   */

    return critical;
}

/**
 * @brief Checks if drone is flying too high.
 *
 */
int ValidateState::HeightControl(int critical)
{
    float errorHeight = (maxHeight_ * 0.66666666);

    if (state_ == 1)
        printf("Error_State\n");
    else
        printf("Normal State \n");

    if ((altitudeSYS_ > maxHeight_) | (altitudeRDS_ > maxHeight_))
    {
        landDrone();
        critical = 1;
    }
    else if (((altitudeSYS_ < errorHeight) & (state_ == 1)) | ((altitudeRDS_ < errorHeight) & (state_ == 1))) // In Error_State State and under 200 m
    {
        state_ = 0;
        printf("Changing state... to Normal\n");
    }
    else if ((altitudeSYS_ > errorHeight) | (altitudeRDS_ > errorHeight))
    {
        // printf("Altitude from pixhawk: %f , Error_Height: %f\n", altitudeSYS_, errorHeight);
        state_ = 1;
        printf("Closing in on ERROR!!! Changing state... Height to Error_State\n");
    }
    return critical;
}

int ValidateState::FreeFall(int critical)
{
    float altitude = (altitudeRDS_ + altitudeSYS_) / 2;
    int time = mymillis();

    float distance = altitude - altitudeRef_;
    float velocity = distance / (time - timeRef_);
    float acceleration = (velocity - velocityRef_ / time - timeRef_);

    if (state_ == 1)
        printf("Error State\n");
    else
        printf("Normal State \n");

    if (acceleration < maxAcceleration_ && FF_IMU_ == 1)
    {
        landDrone(); // Maybe parachute() function here
        critical = 1;
        printf("Closing in on ERROR!!! Changing state... FreeFall to ERROR\n");
    }

    else if ((acceleration < maxAcceleration_) | (FF_IMU_ == 1)) // In Critical State and under 200 m
    {
        state_ = 0;
        printf("Changing state... to Normal\n");
    }

    altitudeRef_ = altitude;
    timeRef_ = time;
    velocityRef_ = velocity;
    return critical;
}

void ValidateState::landDrone()
{
    /*MAVLINK MESSAGE TO LAND*/
    /* if (fail == 3)
    {
        char criticalValue = "Altitude...."
    }

    logger("Critical Value Detected for " + +" Switching to Critical State...\n"); */

    Logger("Landing drone immediately!\n");
}

int ValidateState::mymillis()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

/**
 * @brief Gets current date
 *
 * @param str string value
 * @return std::string returns current date
 */
inline std::string ValidateState::getCurrentDateTime(std::string str)
{
    time_t now = time(0);
    struct tm tstruct;
    char buffer[80];
    tstruct = *localtime(&now);
    if (str == "now")
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %X", &tstruct);
    else if (str == "date")
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tstruct);
    return std::string(buffer);
};

/**
 * @brief Prints inserted string to log file.
 *
 * @param logMsg string value printed to log file
 */
inline void ValidateState::Logger(std::string logMessage)
{
    std::string filePath = "Database/log_" + getCurrentDateTime("date") + ".txt";
    std::string now = getCurrentDateTime("now");
    std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);
    ofs << now << '\t' << logMessage << '\n';
    ofs.close();
}
