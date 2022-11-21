#include "ValidateState.hpp"

ValidateState::~ValidateState() {}

void ValidateState::setState(RDS &RDS, ValidateState *state) 
{
	ValidateState *aux = RDS.state;
	RDS.state = state;
	delete aux;
}

void ValidateState::GetDroneSensorData(RDS &RDS) 
{
  
} 

void ValidateState::LandDrone(RDS &RDS, unsigned int quantity)
{

} // Mavlink

Normal::~Normal() {}

void Normal::AxisControl(RDS &RDS)
{
	setState(RDS, new Normal());
}

void Normal::RouteControl(RDS &RDS) 
{
 	setState(RDS, new Normal());
}

void Normal::HeightControl(RDS &RDS) 
{
 	setState(RDS, new Normal());
}

Critical::~Critical() {}

void Critical::AxisControl(RDS &RDS)
{
	setState(RDS, new Critical());
}

void Critical::RouteControl(RDS &RDS) 
{
 	setState(RDS, new Critical());
}

void Critical::HeightControl(RDS &RDS)  
{
 	setState(RDS, new Critical());
}

HyperCritical::~HyperCritical() {}

void HyperCritical::LandDrone(RDS &RDS)
{
	setState(RDS, new HyperCritical());
}

