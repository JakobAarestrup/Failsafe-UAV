#include "BAR_UNIT.cpp"

int main()
{
    BAR_UNIT b1;

    /**
     * @brief Test variables
     * 
     */
    float temperature[3] = {1,2,3};
    float pressure[3]  = {0,1,2,3};

    /**
     * @brief Temperature and Pressure calculation function
     * 1 for temperature in last variable, anything else for pressure
     */
    float temp_calc = b1.calculatePressureAndTemperature(temperature[1],pressure[1],0);
    float pres_calc = b1.calculatePressureAndTemperature(temperature[1],pressure[1],1);

    /**
     * @brief Initial height in AMSL calculation function
     * Returns the initial height AMSL
     */
    flaot initial_height = b1.initialAMSL(pres_calc);

    /**
     * @brief Compensated height in AGL calculation function
     * Returns the height in AGL
     */
    float height = b1.convertToAGL(pres_calc);

    printf("Calulated temperature: %f\n Calculated pressure: %f\n",temp_calc, pres_calc);
    printf("Initial Height: %f\n Height: %f\n",initial_height, height);
    return 0;
}