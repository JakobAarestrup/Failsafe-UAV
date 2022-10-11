#include "iostream"
#include "string.h"

#pragma once
class GPS
{
    public:
    GPS();
    ~GPS();
    private:
        char[8] lat_;
        char[8] lng_;
};