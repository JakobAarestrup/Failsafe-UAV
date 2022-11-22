#include "SimpleState.hpp"

RDS::RDS(ValidateState* state) : state_(nullptr)
{
    TransitionTo(state);
}

RDS::~RDS()
{ 
    delete state_; 
}

void RDS::UpdateSystemValues()
{
/*Get get get get*/
}

void RDS::LogData()
{
    /*START logging*/
    printf("Longitude: %f %c Latitude: %f %c Satellites: %d Altitude: %f \n"); // GPS Data + baro
    printf("Roll: %f \370 Pitch: %f \370  Yaw: %f \370 \n");  // IMU Data
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
    std::cout << typeid(this).name() << " Yo yocalled AxisControl()\n";
    RDS_->TransitionTo(new Normal);
}

void Normal::RouteControl()
{
    std::cout <<"Normal State: ";
    std::cout << typeid(this).name() << " called RouteControl()\n";
}
void Normal::HeightControl()
{
    std::cout <<"Normal State: ";
    std::cout << typeid(this).name() << " called HeightControl()\n";
}


void Critical::AxisControl()
{
    std::cout <<"Critical State: ";
    std::cout << typeid(this).name() << " Yo yocalled AxisControl()\n";
    RDS_->TransitionTo(new HyperCritical);
}

void Critical::RouteControl()
{
    std::cout <<"Critical State: ";
    std::cout << typeid(this).name() << " called RouteControl()\n";   
}
void Critical::HeightControl()
{
    std::cout <<"Critical State: ";
    std::cout << typeid(this).name() << " called HeightControl()\n";
}

void HyperCritical::AxisControl()
{
    std::cout <<"HyperCritical State: ";
    std::cout << typeid(this).name() << " Yo yocalled AxisControl()\n";
    RDS_->TransitionTo(new Normal);
}

void HyperCritical::RouteControl()
{
    std::cout <<"HyperCritical State: ";
    std::cout << typeid(this).name() << " called RouteControl()\n";
}
void HyperCritical::HeightControl()
{
    std::cout <<"HyperCritical State: ";
    std::cout << typeid(this).name() << " called HeightControl()\n";
}