#include "ValidateState.hpp"

ValidateState::ValidateState()
{
    state_ = 0; // Normal State
}

ValidateState::~ValidateState()
{
}

void ValidateState::GetGPSValues(GPS NEO)
{
    // maybe opret class objekt
    NEO.readGPS();                      // reads NMEA message
    NEO.convertData();                  // converts to decimal degrees format
    longitudeRDS_ = NEO.getLongitude(); // returns longitude
    latitudeRDS_ = NEO.getLatitude();   // returns latitude
    longPoleRDS_ = NEO.getNorthSouth(); // returns either a north pole or south pole
    latPoleRDS_ = NEO.getEastWest();    // returns either a East pole or West pole
    SatellitesRDS_ = NEO.getSV()

    /*MAVLINK GET GPS DATA FROM DRONE*/

    /* longitudeSYS_ =
    latitudeSYS_ =
    longPoleSYS_ =
    latPoleSYS_ =
    SatellitesSYS_ =
    */
}

void ValidateState::GetIMUValues() // void ValidateState::GetIMUValues(IMU Sensor)
{
    /*
    Sensor.readIMU();

    Sensor.ConvertACCData();
    Sensor.ConvertMagData();

    IMU1.ComplementaryFilter();
    RollRDS_ = Sensor.getRoll();
    PitchRDS_ = Sensor.getPitch();
    YawRDS_ = Sensor.getYaw();
    */
    /*MAVLINK GET IMU DATA FROM DRONE*/
    // RollSYS_ =;
    // PitchSYS_ =;
    // YawSYS_ =;
}

void ValidateState::GetBaroValues()
{
    // altitudeRDS_ = getAltitude()
    /*MAVLINK GET BARO DATA FROM DRONE*/
    // altitudeSYS_ =;
}

void ValidateState::UpdateSystemValues(GPS NEO)
{
    GetGPSValues(NEO);
    GetIMUValues();
    GetBaroValues();
}

void ValidateState::LogData()
{
    /*START logging*/
    /*RDS sensors*/
    printf("Longitude: %f %c Latitude: %f %c Satellites: %d Altitude: %f \n", longitudeRDS_, longPoleRDS_, latitudeRDS_, latPoleRDS_, SatellitesRDS_, altitudeRDS_); // GPS Data + baro
    printf("Roll: %f%c Pitch: %f%c  Yaw: %f%c \n", RollRDS_, 176, PitchRDS_, 176, YawRDS_, 176);                                                                     // IMU Data

    /*Drone sensors*/
    printf("Longitude: %f %c Latitude: %f %c Satellites: %d Altitude: %f \n", longitudeSYS_, longPoleSYS_, latitudeSYS_, latPoleSYS_, SatellitesSYS_, altitudeSYS_); // GPS Data + baro
    printf("Roll: %f%c Pitch: %f%c  Yaw: %f%c \n", RollSYS_, 176, PitchSYS_, 176, YawSYS_, 176);                                                                     // IMU Data
}

void ValidateState::AxisControl()
{

    if (state_ == 1)
        printf("Critical State\n");
    else
        printf("Normal State \n");

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

void ValidateState::RouteControl()
{
    if (state_ == 1)
        printf("Critical State\n");
    else
        printf("Normal State \n");

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
void ValidateState::HeightControl()
{
    if (state_ == 1)
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
    }
}

void ValidateState::landDrone()
{
    printf("Landing drone immediately...\n");
    /*MAVLINK MESSAGE TO LAND*/
}
