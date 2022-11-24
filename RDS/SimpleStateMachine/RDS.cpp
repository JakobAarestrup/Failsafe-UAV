
RDS::RDS(ValidateState* state) : state_(nullptr)
{
    TransitionTo(state);
}

RDS::~RDS()
{ 
    delete state_; 
}

void RDS::GetGPSValues()
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
    
    /*MAVLINK GET GPS DATA*/

    /* longitudeSYS_ = 
    latitudeSYS_ = 
    longPoleSYS_ = 
    latPoleSYS_ =  
    SatellitesSYS_ =
    */
}

void RDS::GetIMUValues()
{
    /*
    getRoll();
    getPitch();
    getYaw();
    */
    /*MAVLINK GET IMU FROM DRONE DATA*/
}

void RDS::GetBaroValues()
{
    /*MAVLINK GET BARO DATA*/

}


void RDS::UpdateSystemValues()
{
    GetGPSValues();
    GetIMUValues();
    GetBaroValues();
}

void RDS::LogData()
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

void RDS::TransitionTo(ValidateState* state) 
{
    if (state_) delete state_;
    state_ = state;
    state_->setRDS_(this);
}

void RDS::AnalyseAxis() 
{ 
    state_->AxisControl(); 
}
void RDS::AnalyseRoute() 
{ 
    state_->RouteControl(); 
}

void RDS::AnalyseHeight() 
{ 
    state_->HeightControl(); 
}
