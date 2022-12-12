#include <unistd.h>
#include <iostream>
#include <string>
using namespace std;

void configValidateState(char maxHeight[], char maxAcceleration[], char maxOrientation[], char maxDistance[])
{
    float maxHeight_ = 300;
    float maxAcceleration_ = 15;
    float maxOrientation_ = 45;
    float maxDistance_ = 50;
    if ((strcmp(maxHeight, "Default") == 0))
    {
        return;
    }

    else if (maxHeight != NULL)
    {
        if (atof(maxHeight) != 0)
        {
            maxHeight_ = atof(maxHeight);
            cout << maxHeight_ << "\n";
        }
    }

    if (maxAcceleration != NULL)
    {
        if (atof(maxAcceleration) != 0)
        {
            maxAcceleration_ = atof(maxAcceleration);
            cout << maxAcceleration_ << "\n";
        }
    }

    if (maxOrientation != NULL)
    {
        if (atof(maxOrientation) != 0)
        {
            maxOrientation_ = atof(maxOrientation);
            cout << maxOrientation_ << "\n";
        }
    }

    if (maxDistance != NULL)
    {
        if (atof(maxDistance) != 0)
        {
            maxDistance_ = atof(maxDistance);
            cout << maxDistance_ << "\n";
        }
    }

    cout << "\n"
         << maxHeight_ << "\n";
    cout << maxAcceleration_ << "\n";
    cout << maxOrientation_ << "\n";
    cout << maxDistance_ << "\n";
}

void Usage(const std::string &bin_name)
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

int main(int argc, char **argv)
{
    /**
     * @brief RDS initialized
     *
     */
    char NONE[1];

    if (argc < 3) // minimum arguments 2
    {
        Usage(argv[0]);
        return 1;
    }

    /* cout << "You have entered " << argc
         << " arguments:"
         << "\n";
*/
    for (int i = 0; i < argc; ++i)
    {
        cout << argv[i] << " plads: " << i << endl;
    }
    cout << "\n";
    cout << argv[1] << "\n";
    cout << argv[4] << "\n"
         << endl;

    if ((strcmp(argv[1], argv[4]) == 0))
    {
        configValidateState(argv[2], argv[3], NONE, NONE); // 
    }
    else
        configValidateState(argv[2], argv[3], argv[4], argv[5]);

    /*         else if (argv[2] != NULL)
        {
            if (argc = 2)
            {
                printf("sup\n");
                configValidateState(argv[2], argv[3], NONE, NONE);
            }
            else if (argc >= 3)
            {
                // configValidateState(argv[2], argv[3], argv[4], argv[5]);
                printf("fuck you!\n");
            }
        }*/

    /*  else
     {
         Usage(argv[0]);
         return 1;
     } */

    return 0;
}