#pragma once
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <time.h>
#include <cmath>

class ValidateState
{

public:
  ValidateState();
  ~ValidateState();

  void usage(const std::string &bin_name);
  void configValidateState(char maxHeight[], char maxAcceleration[], char maxOrientation[], char maxDistance[]);

  int mymillis();
  int axisControl(float rollRDS, float rollSYS, float pitchRDS, float pitchSYS, int critical);
  int routeControl(int critical);
  int heightControl(float altitudeRDS, float altitudeSYS, int critical);
  int freeFall(float altitudeRDS, float altitudeSYS, int critical);

private:
  int state_ = 0;
  int ff_IMU_ = 0;

  float maxHeight_ = 300;
  float maxAcceleration_ = 15;
  float maxOrientation_ = 45;
  float maxDistance_ = 50;
  float maxFallSpeed_ = 8.3;

  float altitudeRef_ = 0;
  float velocityRef_ = 0;
  float timeRef_ = 0;
};
