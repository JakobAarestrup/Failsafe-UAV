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
	//.. check values

	//if(roll | pitch | yaw > 0 )
	setState(RDS, new Critical());
	//else if (//.. something)
	//setState(RDS, new HyperCritical());
	//else //.. Dont change state
}

void Normal::RouteControl(RDS &RDS) 
{	
	//.. check values
	
	//if(Routefail > 0 )
 	setState(RDS, new Critical());
	//else if (//.. something)
	setState(RDS, new HyperCritical());
	//else //.. Dont change state
}

void Normal::HeightControl(RDS &RDS) 
{
	//.. check values

	//if (Height > 000) //
 	setState(RDS, new Critical());
	//else if (//.. something)
	setState(RDS, new HyperCritical());
	//else //.. Dont change state
}

Critical::~Critical() {}

void Critical::AxisControl(RDS &RDS)
{
	//.. check values
	printf("Gamer Johnny\n");
	//if(roll | pitch | yaw > 0 ) // checks if hyper critical or Drone is flying okay again
	//setState(RDS, new HyperCritical());
	//else if (//.. something)
	//setState(RDS, new Normal());
	//else //.. Dont change state
}

void Critical::RouteControl(RDS &RDS) 
{
	//.. check values

 	//if(Routefail > 0 ) // checks if hyper critical or Drone is flying okay agaim
 	setState(RDS, new HyperCritical());
	//else if (//.. something)
	setState(RDS, new Normal());
	//else //.. Dont change state
	
}

void Critical::HeightControl(RDS &RDS)  
{
	//.. check values

 	//if (Height > 000) // checks if hyper critical or Drone is flying okay agaim
 	setState(RDS, new HyperCritical());
	//else if (//.. something minorFlag: minor flag er små fejl??)
	setState(RDS, new Normal());
	//else //.. Dont change state

}

HyperCritical::~HyperCritical() {}

void HyperCritical::LandDrone(RDS &RDS)
{
	// Land drone...
}

