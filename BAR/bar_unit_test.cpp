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
    float initial_height;
    float height;

    /**
     * @brief Temperature and Pressure calculation function
     * 1 for temperature in last variable, anything else for pressure
     */
    /*
    float temp_calc = b1.calculatePressureAndTemperature(temperature[1],pressure[1],0);
    float pres_calc = b1.calculatePressureAndTemperature(temperature[1],pressure[1],1);
    */

    /**
     * -100m to -100m
    */
    printf("-------------------------------------\n");
    initial_height = b1.initialAMSL(pressure[0]);
    height = b1.convertToAGL(pressure[0]);
    printf("-100m - -100m test: Initial Height: %f\nHeight: %f\n-------------------------------------\n",initial_height, height);

    /**
     * -100m to 0m
    */
    initial_height = b1.initialAMSL(pressure[0]);
    height = b1.convertToAGL(pressure[1]);
    printf("-100m - 0m test: Initial Height: %f\nHeight: %f\n-------------------------------------\n",initial_height, height);

    /**
     * 0m to 300m
    */
    initial_height = b1.initialAMSL(pressure[1]);
    height = b1.convertToAGL(pressure[2]);
    printf("0m - 300m test: Initial Height: %f\nHeight: %f\n-------------------------------------\n",initial_height, height);

    /**
     * 0m to 500m
    */
    initial_height = b1.initialAMSL(pressure[1]);
    height = b1.convertToAGL(pressure[3]);
    printf("0m - 500m test: Initial Height: %f\nHeight: %f\n-------------------------------------\n",initial_height, height);

    /**
     * -100m to 500m
    */
    initial_height = b1.initialAMSL(pressure[0]);
    height = b1.convertToAGL(pressure[3]);
    printf("-100m - 500m test: Initial Height: %f\nHeight: %f\n-------------------------------------\n",initial_height, height);

    return 0;
}