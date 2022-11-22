#include "ValidateState.hpp"

ValidateState::ValidateState()
{
    state_ = 0; // Normal State
}

ValidateState::~ValidateState()
{ 
    
}

void ValidateState::GetGPSValues()
{
    // maybe opret class objekt
  /*GPS::readGPS(); // reads NMEA message
    GPS::convertData(); //converts to decimal degrees format 
    longitudeRDS_ = GPS::getLongitude();    // returns longitude
    latitudeRDS_ = GPS::getLatitude();      // returns latitude
    longPoleRDS_ = GPS::getNorthSouth();     // returns either a north pole or south pole
    latPoleRDS_ = GPS::getEastWest();       // returns either a East pole or West pole
    SatellitesRDS_ = GPS::getSV()

    */
    
    /*MAVLINK GET GPS DATA FROM DRONE*/

    /* longitudeSYS_ = 
    latitudeSYS_ = 
    longPoleSYS_ = 
    latPoleSYS_ =  
    SatellitesSYS_ =
    */
}

void ValidateState::GetIMUValues()
{
    /*
    RollRDS_ = getRoll();
    PitchRDS_ = getPitch();
    YawRDS_ = getYaw();
    */
    /*MAVLINK GET IMU DATA FROM DRONE*/
    // RollSYS_ =; 
    //PitchSYS_ =; 
    //YawSYS_ =;
   
}

void ValidateState::GetBaroValues()
{
    //altitudeRDS_ = getAltitude()
    /*MAVLINK GET BARO DATA FROM DRONE*/
    //altitudeSYS_ =;
}


void ValidateState::UpdateSystemValues()
{
    GetGPSValues();
    GetIMUValues();
    GetBaroValues();
}

void ValidateState::LogData()
{
    /*START logging*/
    freopen("RDSLog.txt", "w", stdout); // Ved ikke om det her duer ordentligt
    /*RDS sensors*/
    printf("Longitude: %f %c Latitude: %f %c Satellites: %d Altitude: %f \n"
    ,longitudeRDS_, longPoleRDS_, latitudeRDS_, latPoleRDS_, SatellitesRDS_, altitudeRDS_); // GPS Data + baro
    printf("Roll: %f \370 Pitch: %f \370  Yaw: %f \370 \n",RollRDS_, PitchRDS_, YawRDS_);  // IMU Data

    /*Drone sensors*/
    printf("Longitude: %f %c Latitude: %f %c Satellites: %d Altitude: %f \n"
    ,longitudeSYS_, longPoleSYS_, latitudeSYS_, latPoleSYS_, SatellitesSYS_, altitudeSYS_); // GPS Data + baro
    printf("Roll: %f \370 Pitch: %f \370  Yaw: %f \370 \n",RollSYS_, PitchSYS_, YawSYS_);  // IMU Data
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


    if(altitudeSYS_ > 300 | altitudeRDS_ > 300)
    {
        landDrone();
    }

    else if (altitudeSYS_ > 200 | altitudeRDS_ > 200)
    { 
        state_ = 1;
        printf("Closing in on ERROR!!! Changing state... to Critical\n");
    }

    else if (altitudeSYS_ < 200 & state_ == 1 | altitudeRDS_ < 200 & state_ == 1)  // In Critical State and under 200 m
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
