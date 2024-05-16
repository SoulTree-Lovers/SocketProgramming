#include <stdio.h>
#include <pigpio.h>

int main() {
    int pin = 18;

    gpioInitialise();

    gpioSetMode(pin, PI_OUTPUT);
    gpioWrite(pin, 1);
    gpioDelay(1000000);
    gpioWrite(pin, 0);

    gpioTerminate();

    return 0;
}