#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <vector>
#include <mutex>

#include "ValidateStateV2.hpp"
#include "IMU/I2C.hpp"
#include "UDP/UDP.hpp"
#include "BAR/BAR.hpp"
#include "NEO7/Neo7.hpp"
#include "IMU/IMU.hpp"

/**
 * Includes from telemetry example code
 */
#include <iostream>
#include <future>
#include <memory>
#include <thread>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

using namespace mavsdk;
using std::chrono::seconds;
using std::this_thread::sleep_for;

std::shared_ptr<System> get_system(Mavsdk &mavsdk)
{
    std::cout << "Waiting to discover system...\n";
    auto prom = std::promise<std::shared_ptr<System>>{};
    auto fut = prom.get_future();

    // We wait for new systems to be discovered, once we find one that has an
    // autopilot, we decide to use it.
    mavsdk.subscribe_on_new_system([&mavsdk, &prom]()
                                   {
        auto system = mavsdk.systems().back();

        if (system->has_autopilot()) {
            std::cout << "Discovered autopilot\n";

            // Unsubscribe again as we only want to find one system.
            mavsdk.subscribe_on_new_system(nullptr);
            prom.set_value(system);
        } });

    // We usually receive heartbeats at 1Hz, therefore we should find a
    // system after around 3 seconds max, surely.
    if (fut.wait_for(seconds(3)) == std::future_status::timeout)
    {
        std::cerr << "No autopilot found.\n";
        return {};
    }

    // Get discovered system now.
    return fut.get();
}

int mymillis()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

/**
 * @brief logs all read data from the available sensors.
 *
 */

void LogData(Orientation IMUData, GPSPosition GPSData, float altitude, Telemetry::Position position, Telemetry::EulerAngle euler, UDP Client)
{
    /*Values from RDS*/
    float altitudeRDS = altitude;
    float longitudeRDS = GPSData.longitude;
    float latitudeRDS = GPSData.latitude;
    float SatellitesRDS = GPS.SV;

    float RollRDS = IMUData.roll;
    float PitchRDS = IMUData.pitch;
    float YawRDS = IMUData.yaw;

    /*Values over MAVLINK*/
    float altitudeSYS = position.relative_altitude_m;
    float longitudeSYS = position.longitude_deg;
    float latitudeSYS = position.latitude_deg;
    float RollSYS = euler.roll_deg;
    float PitchSYS = euler.pitch_deg;
    float YawSYS = euler.yaw_deg;

    /*START logging*/
    printf("Logging data called..\n");
    /*RDS sensors*/
    std::string GPSBaro = "Longitude: " + std::to_string(longitudeRDS) + " " + GPSData.NS[0] + " Latitude: " + std::to_string(latitudeRDS) + " " + GPSData.EW[0] + " Satellites: " + std::to_string(SatellitesRDS_) + " Altitude: " + std::to_string(altitudeRDS_);
    Logger(GPSBaro);

    std::string IMU = "Roll: " + std::to_string(RollSYS) + " Pitch: " + std::to_string(PitchSYS) + " Yaw: " + std::to_string(YawSYS);
    Logger(IMU);

    std::string GPSBaroSYS = "LongitudeSYS: " + std::to_string(longitudeSYS) + " " + GPSData.NS[0] + " LatitudeSYS: " + std::to_string(latitudeSYS) + " " + GPSData.EW[0] + " AltitudeSYS: " + std::to_string(altitudeSYS);
    Logger(GPSBaroSYS);
    std::string IMUSYS = "RollSYS: " + std::to_string(RollSYS) + " PitchSYS: " + std::to_string(PitchSYS) + " YawSYS: " + std::to_string(YawSYS);
    Logger(IMUSYS);

    // UDP SEND PART
    // char receivedServerMSG[1024];

    std::string RDSData = GPSBaro + IMU;
    std::string SYSData = GPSBaroSYS + IMUSYS;

    const char *RDS = RDSData.c_str();
    const char *SYS = SYSData.c_str();

    Client.UDP_COM(RDS);
    Client.UDP_COM(SYS);
}

/**
 * @brief Gets current date
 *
 * @param str string value
 * @return std::string returns current date
 */
inline std::string ValidateState::getCurrentDateTime(std::string str)
{
    time_t now = time(0);
    struct tm tstruct;
    char buffer[80];
    tstruct = *localtime(&now);
    if (str == "now")
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %X", &tstruct);
    else if (str == "date")
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tstruct);
    return std::string(buffer);
};

/**
 * @brief Prints inserted string to log file.
 *
 * @param logMsg string value printed to log file
 */
inline void ValidateState::Logger(std::string logMessage)
{
    std::string filePath = "Database/log_" + getCurrentDateTime("date") + ".txt";
    std::string now = getCurrentDateTime("now");
    std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);
    ofs << now << '\t' << logMessage << '\n';
    ofs.close();
}

void mainloop(ValidateState &State, BAR &Barometer, Telemetry &telemetry, GPS &G1, IMU &IMU1, IMU &IMU2, UDP &Client)
{
    /*  int loops = 1; */
    int startofloop;

    Telemetry::Position position;
    Telemetry::EulerAngle euler;

    Orientation IMUDATA1;
    Orientation IMUDATA2;
    GPSPosition GPSDATA;

    int altitude = 0;
    int critical = 0;
    float Roll = 0;
    float Pitch = 0;

    while (critical > 0)
    {
        startofloop = mymillis();

        /*Reads data from Barometer and GPS if fifth loop*/
        Barometer.readPressure();
        Barometer.readTemperature();
        Barometer.calculatePressureAndTemperature();

        /* if (loops == 5)
        {
            G1.readGPS();                      // reads NMEA message
            G1.convertData();
            GPSDATA = G1.getGPSPosition();
            loops = 1;
        } */
        // loops++;

        /*Get Data from Sensors*/
        // IMUDATA1 = IMU1.getOrientation();
        IMUDATA2 = IMU2.getOrientation(); // returns IMU Class Struct
        GPSDATA = G1.getGPSPosition();    // returns GPS Class Struc
        altitude = Barometer.getHeight(); // returns altitude

        /*MAVLINK*/
        position = telemetry.position();    // returns struct with values from baro and GPS
        euler = telemetry.attitude_euler(); // returns struct with euler angles

        LogData(GPSDATA, IMUDATA2, altitude, position, euler, Client); // Sends sensor data to log file
        Roll = IMUDATA2.roll;                                          // +IMUDATA1) / 2  // returns
        Pitch = IMUDATA2.pitch;                                        // +IMUDATA1) / 2 // returns

        State.FreeFall(altitude, position.relative_altitude_m, critical);          // Checks error for free fall (acceleration)
        State.AxisControl(Roll, euler.roll_deg, Pitch, euler.pitch_deg, critical); // Checks for error for roll, pitch, and yaw
        State.HeightControl(altitude, position.relative_altitude_m, critical);     // Checks for error for height
        // State.RouteControl(critical); // checks velocity and point and polygon

        printf("Loop Time %d\n", mymillis() - startofloop);
    }

    while (1)
    {
        printf("DRONE IS LANDING!\n");
        State.UpdateSystemValues(Barometer); // gets all values from sensors
        position = telemetry.position();     // returns struct with values from baro and GPS
        euler = telemetry.attitude_euler();  // returns struct with euler angles
        /*Sets all values from MAVLINK*/
        State.SetMAVLinkValues(position.relative_altitude_m, position.latitude_deg, position.longitude_deg,
                               euler.roll_deg, euler.pitch_deg, euler.yaw_deg);
        State.LogData(Client); // Sends sensor data to log file
        /* if{mavdsk register =1)
        exit();
        } */
    }
}

void updateIMUValues(ValidateState &State, IMU &IMU1, IMU &IMU2)
{
    while (1)
    {

        /*Gets Data from first IMU*/
        /* IMU1.readIMU(1);
        IMU1.ConvertACCData();
        IMU1.ConvertMagData();
        IMU1.ComplementaryFilter(); */

        /*Gets Data from second IMU*/
        IMU2.readIMU(2);
        IMU2.ConvertACCData();
        IMU2.ConvertMagData();
        IMU2.ComplementaryFilter();

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

int main(int argc, char **argv)
{

    ValidateState State;

    char NONE[1];

    if (argc < 3) // minimum arguments 2
    {
        State.Usage(argv[0]);
        return 1;
    }

    for (int i = 0; i < argc; ++i)
    {
        std::cout << argv[i] << " plads: " << i << std::endl;
    }
    std::cout << "\n";
    std::cout << argv[1] << "\n";
    std::cout << argv[4] << "\n"
              << std::endl;

    if ((strcmp(argv[1], argv[4]) == 0)) // if you write 20 on serial it
    {
        State.configValidateState(argv[2], argv[3], NONE, NONE);
    }
    else
    {
        State.configValidateState(argv[2], argv[3], argv[4], argv[5]);
    }

    /**
     * @brief Initialize used object variables
     *
     */
    UDP Client;
    GPS G1;
    I2C I1;

    IMU IMU1;
    IMU IMU2;
    BAR B1;
    B1.calibrateBAR(); // 30 seconds from calibration wait time for barometer

    Client.initUDP(); // raspberry pi zero w should get wifi by this time

    /**
     * @brief  Configuration of Sensors
     *
     */
    G1.configAll();     // configs the GPS
    I1.initializeI2C(); // Initialize IMU2 right now but will do both

    /**
     * @brief Calibration..
     *
     */
    //  IMU1.calibrateGyro(1);
    IMU2.calibrateGyro(2);

    /**
     * @brief MAVLINK connection.
     *
     */
    Mavsdk mavsdk;
    ConnectionResult connection_result = mavsdk.add_any_connection(argv[1]);

    if (connection_result != ConnectionResult::Success)
    {
        std::cerr << "Connection failed: " << connection_result << '\n';
        return 1;
    }

    auto system = get_system(mavsdk);
    if (!system)
    {
        return 1;
    }

    // Instantiate plugins.
    auto telemetry = Telemetry{system};
    auto action = Action{system};

    const auto set_rate_position = telemetry.set_rate_position(2.0);
    if (set_rate_position != Telemetry::Result::Success)
    {
        std::cerr << "Setting rate failed: " << set_rate_position << '\n';
        return 1;
    }

    const auto set_rate_euler = telemetry.set_rate_attitude_euler(2.0);
    if (set_rate_euler != Telemetry::Result::Success)
    {
        std::cerr << "Setting rate failed: " << set_rate_euler << '\n';
        return 1;
    }

    /**
     * @brief starting two threads to do main loop and get the IMU data
     */

    std::vector<std::thread> threads;

    threads.push_back(std::thread(mainloop, std::ref(State), std::ref(B1), std::ref(telemetry), std::ref(G1), std::ref(IMU1), std::ref(IMU2), std::ref(Client)));
    threads.push_back(std::thread(updateIMUValues, std::ref(State), std::ref(IMU1), std::ref(IMU2)));

    for (auto &th : threads)
    {
        th.join();
    }

    return 0;
}