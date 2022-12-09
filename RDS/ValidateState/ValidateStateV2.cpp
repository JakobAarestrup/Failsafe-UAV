#include "ValidateState.hpp"

/**
 * @brief Construct a Validate State object
 *
 */
ValidateState::ValidateState()
{
    // state_ = 0; // Normal State
}

/**
 * @brief Destroy the Validate State object
 *
 */
ValidateState::~ValidateState()
{
}

/**
 * @brief Configuration arguments for RDS.exe
 *
 */
void ValidateState::Usage(const std::string &bin_name)
{
    std::cerr << "Usage : " << bin_name << " <serial> <max_height> <max_acceleration> <max_orientation> <max_distance>\n\n"
              << "Serial format should be :\n"
              << " Serial : serial:///path/to/serial/dev[:baudrate]\n"
              << "\n"
              << "Critical Values format should be :\n"
              << " - Max Height       : 0-300 m\n"
              << " - Max Acceleration : 0-25 m/s^2\n"
              << " - Max Orientation  : 0-45 degrees\n"
              << " - Max distance from border : 0-100 m\n"
              << " - Enter Default for : 300 m 15 m/s^2 45 degrees 50 m\n"
              << " - Enter 0s for default values.."
              << " - Example for Serial and Critical Values : serial:///dev/ttyS0:57600 200 10 30 30\n";
}
/**
 * @brief configures the state of the Validate State
 *
 */
void ValidateState::configValidateState(char maxHeight[], char maxAcceleration[], char maxOrientation[], char maxDistance[])
{
    if ((strcmp(maxHeight, "Default") == 0))
    {
        return;
    }

    else if (maxHeight != NULL)
    {
        if (atof(maxHeight) != 0)
        {
            maxHeight_ = atof(maxHeight);
        }
    }

    if (maxAcceleration != NULL)
    {
        if (atof(maxAcceleration) != 0)
        {
            maxAcceleration_ = atof(maxAcceleration);
        }
    }

    if (maxOrientation != NULL)
    {
        if (atof(maxOrientation) != 0)
        {
            maxOrientation_ = atof(maxOrientation);
        }
    }

    if (maxDistance != NULL)
    {
        if (atof(maxDistance) != 0)
        {
            maxDistance_ = atof(maxDistance);
        }
    }

    std::cout << "\n"
              << maxHeight_ << "\n";
    std::cout << maxAcceleration_ << "\n";
    std::cout << maxOrientation_ << "\n";
    std::cout << maxDistance_ << "\n";
}

/**
 * @brief Checks for critical failure for orientation
 *
 */
int ValidateState::AxisControl(float RollRDS, float RollSYS, float PitchRDS, float PitchSYS, int critical)
{
    float errorOrientation = (maxOrientation_ * 0.66666666);

    // Handler til hvis det ikke er et tal?
    /* float Roll = (RollRDS + RollSYS) / 2;
    float Pitch = (PitchRDS + PitchSYS) / 2; */
    float Roll = RollRDS;
    float Pitch = PitchRDS;

    if (state_ == 1)
        printf("Error State\n");
    else
        printf("Normal State \n");

    if ((Roll > maxOrientation_) | (Pitch > maxOrientation_))
    {
        landDrone();
        critical = 1;
    }
    else if (((Roll < errorOrientation) & (state_ == 1)) | ((Pitch < errorOrientation) & (state_ == 1)))
    {
        state_ = 0;
        printf("Changing state... to Normal\n");
    }
    else if ((Roll > errorOrientation) | (Pitch > errorOrientation))
    {
        state_ = 1;
        printf("Changing state... to Error_State Axis\n");
    }
    return critical;
}

/**
 * @brief Checks if the drone is following the correct path
 *
 */
int ValidateState::RouteControl(int critical)
{
    /* if (state_ == 1)
        printf("Error_State\n");
    else
        printf("Normal State \n"); */

    /*   if(altitudeSYS_ > 300 | altitudeRDS_ > 300)
      {
          landDrone();
          critical = 1;
      }

      else if (altitudeSYS_ > 200 | altitudeRDS_ > 200)
      {
          state_ = 1;
          printf("Closing in on ERROR!!! Changing state... to Error_State\n");
      }

      else if (altitudeSYS_ < 200 | altitudeRDS_ < 200)
      {
          state_ = 0;
          printf("Changing state... to Normal\n");
      }
      return critical;
   */

    return critical;
}

/**
 * @brief Checks if drone is flying too high.
 *
 */
int ValidateState::HeightControl(float altitudeRDS, float altitudeSYS, int critical)
{
    float errorHeight = (maxHeight_ * 0.66666666);

    if (state_ == 1)
        printf("Error_State\n");
    else
        printf("Normal State \n");

    if ((altitudeSYS > maxHeight_) | (altitudeRDS > maxHeight_))
    {
        landDrone();
        critical = 1;
    }
    else if (((altitudeSYS < errorHeight) & (state_ == 1)) | ((altitudeRDS < errorHeight) & (state_ == 1))) // In Error_State State and under 200 m
    {
        state_ = 0;
        printf("Changing state... to Normal\n");
    }
    else if ((altitudeSYS > errorHeight) | (altitudeRDS_ > errorHeight))
    {
        // printf("Altitude from pixhawk: %f , Error_Height: %f\n", altitudeSYS_, errorHeight);
        state_ = 1;
        printf("Closing in on ERROR!!! Changing state... Height to Error_State\n");
    }
    return critical;
}

int ValidateState::FreeFall(float altitudeRDS, float altitudeSYS, int critical)
{
    float altitude = (altitudeRDS + altitudeSYS) / 2;
    int time = mymillis();

    float distance = altitude - altitudeRef_;
    float velocity = distance / (time - timeRef_);
    float acceleration = (velocity - velocityRef_ / time - timeRef_);

    if (state_ == 1)
        printf("Error State\n");
    else
        printf("Normal State \n");

    if (acceleration < maxAcceleration_ && FF_IMU_ == 1)
    {
        landDrone(); // Maybe parachute() function here
        critical = 1;
        printf("Closing in on ERROR!!! Changing state... FreeFall to ERROR\n");
    }

    else if ((acceleration < maxAcceleration_) | (FF_IMU_ == 1)) // In Critical State and under 200 m
    {
        state_ = 0;
        printf("Changing state... to Normal\n");
    }

    altitudeRef_ = altitude;
    timeRef_ = time;
    velocityRef_ = velocity;
    return critical;
}

void ValidateState::landDrone()
{
    /*MAVLINK MESSAGE TO LAND*/
    /* if (fail == 3)
    {
        char criticalValue = "Altitude...."
    }

    logger("Critical Value Detected for " + +" Switching to Critical State...\n"); */

    Logger("Landing drone immediately!\n");
}

int ValidateState::mymillis()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}
