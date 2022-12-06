cmake_minimum_required(VERSION 3.15.2)

# Specify your project's name
project(RDSV2)

# Specify at least C++11, better C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

# Enable strict handling of warnings
add_definitions("-Wall -Wextra -Werror")

# Finds MAVSDK when installed system wide.
find_package(MAVSDK REQUIRED)
find_package(Threads REQUIRED)

# Specify your app's executable name, and list of source files used to create it.
# change first and last line in add_executable to the desired configuration for RDS with or without GPS
add_executable(RDSV2
    main.cpp
    NEO7/Neo7.cpp
    IMU/IMU.cpp
    IMU/I2C.cpp
    IMU/I2Cdev.cpp
    BAR/BAR.cpp
    UDP/UDP.cpp
    ValidateState.cpp
    # ... any other source files
)


find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
# Specify your app's executable name and a list of linked libraries
target_link_libraries(RDS
PRIVATE Threads::Threads
MAVSDK::mavsdk

${WIRINGPI_LIBRARIES}    # ... Any other linked libraries
)