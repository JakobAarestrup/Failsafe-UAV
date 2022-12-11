#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <vector>
#include <mutex>

#include "ValidateState/ValidateState.hpp"
#include "IMU/I2C.hpp"
#include "UDP/UDP.hpp"
#include "BAR/BAR.hpp"
#include "GPS/GPS.hpp"
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

/**
 * @brief Function connects to drone via MAVLink
 *
 * @param mavsdk
 * @return std::shared_ptr<System>
 */
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

/**
 * @brief Function that tells time passed since boot
 *
 * @return int time since boot
 */
int mymillis()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

/**
 * @brief Function gets current date
 *
 * @param str string value
 * @return std::string
 */
inline std::string getCurrentDateTime(std::string str)
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
 * @brief Function prints inserted string to log file.
 *
 * @param logMsg string value printed to log file
 */
inline void Logger(std::string logMessage)
{
    std::string filePath = "Database/log_" + getCurrentDateTime("date") + ".txt";
    std::string now = getCurrentDateTime("now");
    std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);
    ofs << now << '\t' << logMessage << '\n';
    ofs.close();
}

/**
 * @brief Function logs all read data from the available sensors.
 *
 * @param GPSData is the data from the GPS
 * @param IMUData is the data from the IMU
 * @param altitude is the altitude from the Barometer
 * @param position is the position data from the drone
 * @param euler is the euler data from the drone
 * @param Client is the UDP client member
 */
void LogData(GPSPosition GPSData, Orientation IMUData, float altitude, Telemetry::Position position, Telemetry::EulerAngle euler, UDP Client)
{
    /*Values from RDS*/
    float altitudeRDS = altitude;
    float longitudeRDS = GPSData.longitude;
    float latitudeRDS = GPSData.latitude;
    float SatellitesRDS = GPSData.SV;

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
    // printf("Logging data called..\n");
    /*RDS sensors*/
    std::string GPSBaro = "Longitude: " + std::to_string(longitudeRDS) + " " + GPSData.NS[0] + " Latitude: " + std::to_string(latitudeRDS) + " " + GPSData.EW[0] + " Satellites: " + std::to_string(SatellitesRDS) + " Altitude: " + std::to_string(altitudeRDS);
    Logger(GPSBaro);

    std::string IMU = "Roll: " + std::to_string(RollRDS) + " Pitch: " + std::to_string(PitchRDS) + " Yaw: " + std::to_string(YawRDS);
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
 * @brief Function runs the main loop of RDS
 *
 * @param State is the current state of RDS
 * @param Barometer is the Barometer object
 * @param telemetry is the telemetry object from the drone
 * @param G1 is the GPS object
 * @param IMU1 is the IMU object
 * @param Client is the UDP client object
 */
void mainloop(ValidateState &State, BAR &Barometer, Telemetry &telemetry, GPS &G1, IMU &IMU1, UDP &Client) // IMU &IMU1,
{
    int loops = 1;
    int startofloop;

    Telemetry::Position position;
    Telemetry::EulerAngle euler;

    Orientation IMUDATA1;
    // Orientation IMUDATA2;
    GPSPosition GPSDATA;

    float altitude = 0;
    float Roll = 0;
    float Pitch = 0;

    int critical = 0;

    G1.readGPS(); // reads NMEA message
    G1.convertData();
    GPSDATA = G1.getGPSPosition();

    while (critical > 0)
    {
        startofloop = mymillis();

        /*Reads data from Barometer and GPS if fifth loop*/
        Barometer.readPressure();
        Barometer.readTemperature();
        Barometer.calculatePressureAndTemperature();

        if (loops == 5)
        {
            G1.readGPS(); // reads NMEA message
            G1.convertData();
            GPSDATA = G1.getGPSPosition();
            loops = 1;
        }
        // loops++;

        /*Get Data from Sensors*/
        IMUDATA1 = IMU1.getOrientation();
        // IMUDATA2 = IMU2.getOrientation(); // returns IMU Class Struct
        // GPSDATA = G1.getGPSPosition();    // returns GPS Class Struc
        altitude = Barometer.getHeight(); // returns altitude

        /*MAVLINK*/
        position = telemetry.position();    // returns struct with values from baro and GPS
        euler = telemetry.attitude_euler(); // returns struct with euler angles

        LogData(GPSDATA, IMUDATA1, altitude, position, euler, Client); // Sends sensor data to log file
        Roll = IMUDATA1.roll;                                          //(IMUDATA2.roll + IMUDATA1.roll) / 2;                    // returns
        Pitch = IMUDATA1.pitch;                                        //(IMUDATA2.pitch + IMUDATA1.pitch) / 2;                 // returns
        std::cout << "Loop Time: " << mymillis() - startofloop << std::endl;
        State.freeFall(altitude, position.relative_altitude_m, critical);          // Checks error for free fall (acceleration)
        State.axisControl(Roll, euler.roll_deg, Pitch, euler.pitch_deg, critical); // Checks for error for roll, pitch, and yaw
        State.heightControl(altitude, position.relative_altitude_m, critical);     // Checks for error for height
        // State.routeControl(critical); // checks velocity and point and polygon
    }

    while (1)
    {
        /*Reads data from Barometer and GPS if fifth loop*/
        Barometer.readPressure();
        Barometer.readTemperature();
        Barometer.calculatePressureAndTemperature();

        if (loops == 5)
        {
            G1.readGPS(); // reads NMEA message
            G1.convertData();
            GPSDATA = G1.getGPSPosition();
            loops = 1;
        }
        // loops++;

        /*Get Data from Sensors*/
        // IMUDATA1 = IMU1.getOrientation();
        IMUDATA1 = IMU1.getOrientation(); // returns IMU Class Struct
        GPSDATA = G1.getGPSPosition();    // returns GPS Class Struc
        altitude = Barometer.getHeight(); // returns altitude

        /*MAVLINK*/
        position = telemetry.position();    // returns struct with values from baro and GPS
        euler = telemetry.attitude_euler(); // returns struct with euler angles

        LogData(GPSDATA, IMUDATA1, altitude, position, euler, Client); // Sends sensor data to log file
        Roll = IMUDATA1.roll;                                          // +IMUDATA1) / 2  // returns
        Pitch = IMUDATA1.pitch;
        /* if{mavdsk register =1)
        exit();
        } */
    }
}

/**
 * @brief Function updates the values from the IMU
 *
 * @param IMU2 is the IMU object
 */
void updateIMUValues(IMU &IMU2) // IMU &IMU1,
{
    while (1)
    {

        /*Gets Data from first IMU*/
        /*
        IMU1.readIMU(1);
        IMU1.convertAccData();
        IMU1.convertMagData();
        IMU1.complementaryFilter();
        IMU1.freeFall()
        */

        /*Gets Data from second IMU*/
        IMU2.readIMU(1);
        IMU2.convertAccData();
        IMU2.convertMagData();
        IMU2.complementaryFilter();
        // IMU2.freeFall()
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

/**
 * @brief Main code
 *
 * @param argc Input argument for setting limit values
 * @param argv Input argument for setting limit values
 * @return int 0
 */
int main(int argc, char **argv)
{

    ValidateState State;

    char NONE[1];

    if (argc < 3) // minimum arguments 2
    {
        State.usage(argv[0]);
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
    // IMU IMU2;
    BAR B1;
    B1.calibrateBAR(); // 30 seconds from calibration wait time for barometer

    Client.initUDP(); // raspberry pi zero w should get wifi by this time

    /// @brief Configuration of Sensors
    G1.configAll();     // configs the GPS
    I1.initializeI2C(); // Initialize IMU2 right now but will do both

    /// @brief Calibration
    // IMU1.calibrateGyro(1);
    IMU1.calibrateGyro(1);

    /// @brief MAVLINK connection.
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

    /// @brief Instantiate plugins
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

    threads.push_back(std::thread(mainloop, std::ref(State), std::ref(B1), std::ref(telemetry), std::ref(G1), std::ref(IMU1), std::ref(Client))); // std::ref(IMU2),
    threads.push_back(std::thread(updateIMUValues, std::ref(IMU1)));

    for (auto &th : threads)
    {
        th.join();
    }

    return 0;
}