#include "BAR_UNIT.hpp"

int main()
{
    BAR_UNIT b1;

    /**
     * @brief Test variables
     * 
     */
    //float temperature[3] = {1,2,3};
    float pressure[4]  = {102532.096,101325,97772.574,95460.834}; //-100m, 0m, 300m, 500m

    /**
     * @brief Temperature and Pressure calculation function
     * 1 for temperature in last variable, anything else for pressure
     */
    /*
    float temp_calc = b1.calculatePressureAndTemperature(temperature[1],pressure[1],0);
    float pres_calc = b1.calculatePressureAndTemperature(temperature[1],pressure[1],1);
    */

    /**
     * @brief Initial height in AMSL calculation function
     * Returns the initial height AMSL
     */
    float initial_height = b1.initialAMSL(pressure[0]);

    /**
     * @brief Compensated height in AGL calculation function
     * Returns the height in AGL
     */
    float height = b1.convertToAGL(pressure[1]);

    printf("Initial Height: %f\nHeight: %f\n",initial_height, height);
    return 0;
}