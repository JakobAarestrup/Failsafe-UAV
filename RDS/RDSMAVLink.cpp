
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

void quaternionToEuler(Telemetry::Quaternion &q, float &roll, float &pitch, float &yaw)
{
    // Compute roll
    float sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    float cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    roll = std::atan2(sinr_cosp, cosr_cosp);

    // Compute pitch
    float sinp = 2 * (q.w * q.y - q.z * q.x);

    if (std::abs(sinp) >= 1)
        pitch = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        pitch = std::asin(sinp);

    // Compute yaw
    float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    yaw = std::atan2(siny_cosp, cosy_cosp);

    // Convert angles to degrees
    roll = roll * 180 / M_PI;
    pitch = pitch * 180 / M_PI;
    yaw = (yaw * 180 / M_PI) + 180;
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
 * @param q is the quaternion data from the drone
 * @param Client is the UDP client member
 */
void LogData(Orientation IMUData, float altitude, Telemetry::Position position, float rollSYS, float pitchSYS, float yawSYS, UDP Client)
{
    /*Values from RDS*/
    float altitudeRDS = altitude;

    float rollRDS = IMUData.roll;
    float pitchRDS = IMUData.pitch;
    float yawRDS = IMUData.yaw;

    /*Values over MAVLINK*/
    float altitudeSYS = position.relative_altitude_m;
    float longitudeSYS = position.longitude_deg;
    float latitudeSYS = position.latitude_deg;

    /*RDS sensors*/
    std::string GPSBaro = "Longitude: 0, N, Latitude: 0, E, Satellites: 0, Altitude: " + std::to_string(altitudeRDS);
    Logger(GPSBaro);

    std::string IMU = " Roll: " + std::to_string(rollRDS) + " Pitch: " + std::to_string(pitchRDS) + " Yaw: " + std::to_string(yawRDS);
    Logger(IMU);

    std::string GPSBaroSYS = "LongitudeSYS: " + std::to_string(longitudeSYS) + " " + "N" + " LatitudeSYS: " + std::to_string(latitudeSYS) + " " + "E" + " AltitudeSYS: " + std::to_string(altitudeSYS);
    Logger(GPSBaroSYS);
    std::string IMUSYS = " RollSYS: " + std::to_string(rollSYS) + " PitchSYS: " + std::to_string(pitchSYS) + " YawSYS: " + std::to_string(yawSYS);
    Logger(IMUSYS);

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

void mainloop(ValidateState &State, BAR &Barometer, Telemetry &telemetry, IMU &IMU1, UDP &Client, Action &action) // Action &action) // IMU &IMU1,
{

    int startofloop;

    Telemetry::Position position;
    Telemetry::Quaternion q;

    Orientation IMUDATA1;
    // Orientation IMUDATA2;
    // GPSPosition GPSDATA;

    float altitude = 0;
    float roll = 0;
    float pitch = 0;
    float q_Roll = 0;
    float q_Pitch = 0;
    float q_Yaw = 0;

    int critical = 0;

    /* G1.readGPS(); // reads NMEA message
    G1.convertData();
    GPSDATA = G1.getGPSPosition(); */
    // GPSDATA = G1.getGPSPosition();
    while (critical < 1)
    {
        startofloop = mymillis();

        // Reads data from Barometer and GPS if fifth loop
        Barometer.readPressure();
        Barometer.readTemperature();
        Barometer.calculatePressureAndTemperature();

        /* if (loops == 5)
        {
            G1.readGPS(); // reads NMEA message
            G1.convertData();
            GPSDATA = G1.getGPSPosition();
            loops = 1;
        }
        loops++; */
        // GPSDATA = G1.getGPSPosition();
        //  Get Data from Sensors
        IMUDATA1 = IMU1.getOrientation();
        // IMUDATA2 = IMU2.getOrientation(); // returns IMU Class Struct

        altitude = Barometer.getHeight(); // returns altitude */

        // MAVLINK
        position = telemetry.position();              // returns struct with values from baro and GPS
        q = telemetry.attitude_quaternion();          // returns struct with euler angles
        quaternionToEuler(q, q_Roll, q_Pitch, q_Yaw); // get quaternions in degrees
        //  logging Data
        LogData(IMUDATA1, altitude, position, q_Roll, q_Pitch, q_Yaw, Client); // Sends sensor data to log file
        // Analyse State
        roll = IMUDATA1.roll; //(IMUDATA2.roll + IMUDATA1.roll) / 2;                    // returns
        pitch = IMUDATA1.pitch;
        //(IMUDATA2.pitch + IMUDATA1.pitch) / 2;                 // returns

        // State.freeFall(altitude, position.relative_altitude_m, critical);      // Checks error for free fall (acceleration)
        critical = State.axisControl(roll, q_Roll, pitch, q_Pitch, critical);             // Checks for error for roll, pitch, and yaw
        critical = State.heightControl(altitude, position.relative_altitude_m, critical); // Checks for error for height
                                                                                          // State.routeControl(critical); // checks velocity and point and polygon */
        std::cout << "Loop Time: " << mymillis() - startofloop << " Critical: " << critical << std::endl;
    }

    const Action::Result land_result = action.land();
    if (land_result != Action::Result::Success)
    {
        std::cerr << "Couldn't land drone: " << land_result << '\n';
    }

    Logger("Critical Failure Detected\n");
    Client.UDP_COM("Critical Failure Detected\n");
    while (1)
    {
        startofloop = mymillis();

        // Reads data from Barometer and GPS if fifth loop
        Barometer.readPressure();
        Barometer.readTemperature();
        Barometer.calculatePressureAndTemperature();

        /* if (loops == 5)
        {
            G1.readGPS(); // reads NMEA message
            G1.convertData();
            GPSDATA = G1.getGPSPosition();
            loops = 1;
        }
        loops++; */
        // GPSDATA = G1.getGPSPosition();
        //  Get Data from Sensors
        IMUDATA1 = IMU1.getOrientation();
        // IMUDATA2 = IMU2.getOrientation(); // returns IMU Class Struct

        altitude = Barometer.getHeight(); // returns altitude */

        // MAVLINK
        position = telemetry.position();              // returns struct with values from baro and GPS
        q = telemetry.attitude_quaternion();          // returns struct with euler angles
        quaternionToEuler(q, q_Roll, q_Pitch, q_Yaw); // get quaternions in degrees
        //  logging Data
        LogData(IMUDATA1, altitude, position, q_Roll, q_Pitch, q_Yaw, Client); // Sends sensor data to log file
    }
}

/**
 * @brief Function updates the values from the IMU
 *
 * @param IMU2 is the IMU object
 */
void updateIMUValues(IMU &IMU1) // IMU &IMU1,
{
    while (1)
    {

        /*Gets Data from first IMU*/

        IMU1.readIMU(1);
        IMU1.convertAccData();
        IMU1.convertMagData();
        IMU1.complementaryFilter();
        // IMU1.freeFall(1)
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        /*Gets Data from second IMU*/
        /*  IMU2.readIMU(1);
         IMU2.convertAccData();
         IMU2.convertMagData();
         IMU2.complementaryFilter();
         // IMU2.freeFall(2) */
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
    I2C I1;

    IMU IMU1;
    // IMU IMU2;
    BAR B1;
    B1.calibrateBAR(); // 30 seconds from calibration wait time for barometer

    Client.initUDP(); // raspberry pi zero w should get wifi by this time

    /// @brief Configuration of Sensors
    // G1.configAll();     // configs the GPS
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

    // We want to listen to the altitude of the drone at 1 Hz.
    const auto set_rate_result = telemetry.set_rate_position(2.0);
    if (set_rate_result != Telemetry::Result::Success)
    {
        std::cerr << "Setting rate failed: " << set_rate_result << '\n';
        return 1;
    }

    const auto set_rate_result1 = telemetry.set_rate_attitude_quaternion(2.0);
    if (set_rate_result1 != Telemetry::Result::Success)
    {
        std::cerr << "Setting rate failed: " << set_rate_result1 << '\n';
        return 1;
    }

    /**
     * @brief starting two threads to do main loop and get the IMU data
     */

    std::vector<std::thread> threads;

    threads.push_back(std::thread(mainloop, std::ref(State), std::ref(B1), std::ref(telemetry), std::ref(IMU1), std::ref(Client), std::ref(action))); // std::ref(IMU2),
    threads.push_back(std::thread(updateIMUValues, std::ref(IMU1)));                                                                                  //,std::ref(IMU2))

    for (auto &th : threads)
    {
        th.join();
    }
    return 0;
}