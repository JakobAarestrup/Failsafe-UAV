
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <cstddef>
#include <unistd.h>
#include <iostream>
#include "Johnny.hpp"
#include <fstream>
#include <sstream>

int main()
{
    int number = 0;
    std::string johnny = std::to_string(number) + "gamer";
    Logger(johnny);
    return 0;
}
