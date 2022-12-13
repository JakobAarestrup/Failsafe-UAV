#include "BAR_UNIT.hpp"

int main()
{
    BAR_UNIT b1;

    /**
     * @brief Test variables
     */
    float pressure[4]  = {102532.096,101325,97772.574,95460.834}; //-100m, 0m, 300m, 500m
    float initial_height;
    float height;

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