#include <stdio.h>
#include <pigpio.h>

int main() {
    int led1 = 12;
    int led2 = 13;

    gpioInitialise();

    gpioHardwarePWM(led1, 1, 500000);
    gpioHardwarePWM(led2, 2, 500000);

    gpioTerminate();

    return 0;
}