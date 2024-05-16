#include <stdio.h>
#include <pigpio.h>

int main() {
    int pin = 18;
    int switch_pin = 24;

    if (gpioInitialise() < 0) {
        printf("Failed to initial\n");
        return 1;
    }

    gpioSetMode(pin, PI_OUTPUT);
    gpioSetMode(switch_pin, PI_INPUT);

    while(1) {
        if (gpioRead(switch_pin) == 0) {
            gpioWrite(pin, 1);
            gpioDelay(100000);
            gpioWrite(pin, 0);
        }
    }

    gpioTerminate();

    return 0;
}