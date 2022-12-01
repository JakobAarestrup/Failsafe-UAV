cmake_minimum_required(VERSION 3.15.2)

# Specify your project's name
project(RDS)

# Specify at least C++11, better C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable strict handling of warnings
add_definitions("-Wall -Wextra -Werror")

# Finds MAVSDK when installed system wide.
find_package(MAVSDK REQUIRED)

# Specify your app's executable name, and list of source files used to create it.
add_executable(RDS
    main.cpp
    NEO7/Neo7.cpp
    IMU/IMU.cpp
    IMU/I2C.cpp
    IMU/I2Cdev.cpp
    BAR/BAR.cpp
    ValidateState.cpp
    # ... any other source files
)

find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
# Specify your app's executable name and a list of linked libraries
target_link_libraries(RDS
MAVSDK::mavsdk
${WIRINGPI_LIBRARIES}    # ... Any other linked libraries
)