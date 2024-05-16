#include <stdio.h>
#include <pigpio.h>

int main() {
    int yellow_pin = 12;
    int red_pin = 13;
    int green_pin = 16;

    gpioInitialise();

    gpioSetMode(yellow_pin, PI_OUTPUT);
    gpioSetMode(red_pin, PI_OUTPUT);
    gpioSetMode(green_pin, PI_OUTPUT);

    while(1) {
            // Turn on yellow LED for 3 seconds
            gpioWrite(yellow_pin, 1);
            gpioWrite(red_pin, 0);
            gpioWrite(green_pin, 0);
            gpioDelay(3000000); // 3 seconds

            // Turn on red LED for 3 seconds
            gpioWrite(yellow_pin, 0);
            gpioWrite(red_pin, 1);
            gpioWrite(green_pin, 0);
            gpioDelay(3000000); // 3 seconds

            // Turn on green LED for 3 seconds
            gpioWrite(yellow_pin, 0);
            gpioWrite(red_pin, 0);
            gpioWrite(green_pin, 1);
            gpioDelay(3000000); // 3 seconds
    }

    gpioTerminate();

    return 0;
}