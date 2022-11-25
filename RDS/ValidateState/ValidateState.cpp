#include "ValidateState.hpp"

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

    /*MAVLINK GET GPS DATA FROM DRONE*/

    /* longitudeSYS_ =
    latitudeSYS_ =
    longPoleSYS_ =
    latPoleSYS_ =
    SatellitesSYS_ =
    */
}

/**
 * @brief Gets Roll, Pitch and YAW from IMU object and over MAVLINK from Drone
 *
 * @param sensor Class object of IMU class
 */
void ValidateState::GetIMUValues(IMU sensor) // void ValidateState::GetIMUValues(IMU sensor)
{

    sensor.readIMU(2);
    sensor.ConvertACCData();
    sensor.ConvertMagData();
    sensor.ComplementaryFilter();

    RollRDS_ = sensor.getRoll();
    PitchRDS_ = sensor.getPitch();
    YawRDS_ = sensor.getYaw();

    /*MAVLINK GET IMU DATA FROM DRONE*/
    // RollSYS_ =;
    // PitchSYS_ =;
    // YawSYS_ =;
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

    /*MAVLINK GET BARO DATA FROM DRONE*/
    // altitudeSYS_ =;
}

/**
 * @brief Updates all private variables in ValidateState for critical failure status.
 *
 * @param NEO Class object of GPS class
 * @param barometer  Class object of BAR class
 * @param sensor Class object of IMU class
 */
void ValidateState::UpdateSystemValues(GPS NEO, BAR barometer, IMU sensor)
{
    // printf("Hello there updating values...\n");
    GetBaroValues(barometer);
    GetIMUValues(sensor);
    usleep(100);
    GetGPSValues(NEO);
}

/**
 * @brief logs all read data from the available sensors.
 *
 */
void ValidateState::LogData()
{
    /*START logging*/
    /*RDS sensors*/
    std::string GPSBaro = "Longitude: " + std::to_string(longitudeRDS_) + " " + longPoleRDS_[0] + " Latitude: " + std::to_string(latitudeRDS_) + " " + latPoleRDS_ + " Satellites: " + std::to_string(SatellitesRDS_) + " Altitude: " + std::to_string(altitudeRDS_);
    Logger(GPSBaro);
    std::string IMU = "Roll: " + std::to_string(RollRDS_) + " Pitch: " + std::to_string(PitchRDS_) + " Yaw: " + std::to_string(YawRDS_);
    Logger(IMU);
    /*  Logger("Longitude: %f %c Latitude: %f %c Satellites: %d Altitude: %f \n", longitudeRDS_, longPoleRDS_, latitudeRDS_, latPoleRDS_, SatellitesRDS_, altitudeRDS_); // GPS Data + baro
     printf("Roll: %f%c Pitch: %f%c  Yaw: %f%c \n", RollRDS_, 176, PitchRDS_, 176, YawRDS_, 176);  */
    // IMU Data

    /*
    printf("Longitude: %f %c Latitude: %f %c Satellites: %d Altitude: %f \n", longitudeSYS_, longPoleSYS_, latitudeSYS_, latPoleSYS_, SatellitesSYS_, altitudeSYS_); // GPS Data + baro
    printf("Roll: %f%c Pitch: %f%c  Yaw: %f%c \n", RollSYS_, 176, PitchSYS_, 176, YawSYS_, 176);  */
    // IMU Data
}

/**
 * @brief Checks for critical failure for orientation
 *
 */
void ValidateState::AxisControl()
{

    /*  if (state_ == 1)
         printf("Critical State\n");
     else
         printf("Normal State \n"); */

    /*   if(altitudeSYS_ > 300 | altitudeRDS_ > 300)
  {
      landDrone();
  }

  else if (altitudeSYS_ > 200 | altitudeRDS_ > 200)
  {
      state_ = 1;
      printf("Closing in on ERROR!!! Changing state... to Critical\n");
  }

  else if (altitudeSYS_ < 200 | altitudeRDS_ < 200)
  {
      state_ = 0;
      printf("Changing state... to Normal\n");
  }
*/
}

/**
 * @brief Checks if the drone is following the correct path
 *
 */
void ValidateState::RouteControl()
{
    /* if (state_ == 1)
        printf("Critical State\n");
    else
        printf("Normal State \n"); */

    /*   if(altitudeSYS_ > 300 | altitudeRDS_ > 300)
      {
          landDrone();
      }

      else if (altitudeSYS_ > 200 | altitudeRDS_ > 200)
      {
          state_ = 1;
          printf("Closing in on ERROR!!! Changing state... to Critical\n");
      }

      else if (altitudeSYS_ < 200 | altitudeRDS_ < 200)
      {
          state_ = 0;
          printf("Changing state... to Normal\n");
      }
   */
}

/**
 * @brief Checks if drone is flying too high.
 *
 */
void ValidateState::HeightControl()
{
    /*     if (state_ == 1)
            printf("Critical State\n");
        else
            printf("Normal State \n");

        if (altitudeSYS_ > 300 | altitudeRDS_ > 300)
        {
            landDrone();
        }

        else if (altitudeSYS_ > 200 | altitudeRDS_ > 200)
        {
            state_ = 1;
            printf("Closing in on ERROR!!! Changing state... to Critical\n");
        }

        else if (altitudeSYS_ < 200 & state_ == 1 | altitudeRDS_ < 200 & state_ == 1) // In Critical State and under 200 m
        {
            state_ = 0;
            printf("Changing state... to Normal\n");
        } */
}

void ValidateState::landDrone()
{
    printf("Landing drone immediately...\n");
    /*MAVLINK MESSAGE TO LAND*/
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
