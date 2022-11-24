#include "SimpleState.hpp"

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
    fp = freopen("RDSLog.txt", "w", stdout); // Ved ikke om det her duer ordentligt
    /*RDS sensors*/
    printf("Longitude: %f %c Latitude: %f %c Satellites: %d Altitude: %f \n"
    ,longitudeRDS_, longPoleRDS_, latitudeRDS_, latPoleRDS_, SatellitesRDS_, altitudeRDS_); // GPS Data + baro
    printf("Roll: %f \370 Pitch: %f \370  Yaw: %f \370 \n",RollRDS_, PitchRDS_, YawRDS_);  // IMU Data

    /*Drone sensors*/
    printf("Longitude: %f %c Latitude: %f %c Satellites: %d Altitude: %f \n"
    ,longitudeSYS_, longPoleSYS_, latitudeSYS_, latPoleSYS_, SatellitesSYS_, altitudeSYS_); // GPS Data + baro
    printf("Roll: %f \370 Pitch: %f \370  Yaw: %f \370 \n",RollSYS_, PitchSYS_, YawSYS_);  // IMU Data
    fclose(fp);
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

void Normal::AxisControl()
{
    std::cout <<"Normal State: ";
   // std::cout << typeid(this).name() << "called AxisControl()\n"; gets pointer value and class name
    RDS_->TransitionTo(new Normal);
}

void Normal::RouteControl()
{
    std::cout <<"Normal State: ";
    //.. Compare values
	//if(Routefail > 0 
    RDS_->TransitionTo(new Critical);
	//else if (//.. something)
	RDS_->TransitionTo(new Critical);
	//else //.. Dont change state
}
void Normal::HeightControl()
{
    std::cout <<"Normal State: ";
    /* if(altitudeSYS_ > 300 | altitudeRDS_ > 300)
    {
        landDrone();
    }

    else if (altitudeSYS_ > 200 | altitudeRDS_ > 200)
    { 
        RDS_->TransitionTo(new Critical);
    }
    */
}


void Critical::AxisControl()
{
    std::cout <<"Critical State: ";
    RDS_->TransitionTo(new Critical);
}

void Critical::RouteControl()
{
    std::cout <<"Critical State: ";
    //.. Compare values
	//if(Routefail > 0 
    RDS_->TransitionTo(new Normal);
	//else if (//.. something)
	//else //.. Dont change state
}
    
void Critical::HeightControl()
{
   /*  std::cout <<"Critical State: ";
    if (altitudeSYS_ > 300 | altitudeRDS_ > 300)
    {
	    landDrone();
    }
    else if (altitudeSYS_ < 200 | altitudeRDS_ < 200)
    {
        RDS_->TransitionTo(new Normal);
    } */
    
}

void Critical::landDrone()
{
    printf("Landing drone immediately...");
    /*MAVLINK MESSAGE TO LAND*/
}
/* void HyperCritical::AxisControl() {}
void HyperCritical::RouteControl() {}
void HyperCritical::HeightControl() {}

void HyperCritical::landDrone()
{
    printf("Landing drone immediately...")
    /*MAVLINK MESSAGE TO LAND
} 
*/