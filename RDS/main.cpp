// main.cpp
#include "RDS.hpp"
#include "ValidateState.hpp"
#include <iostream>
#include <stdexcept>

int main() 
{
    RDS DroneSystem;
    while(1)
    {

        DroneSystem.LogData();

    }

    return 0;
}

//https://stackoverflow.com/questions/14676709/c-code-for-state-machine