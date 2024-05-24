#include <stdio.h>
#include <pigpio.h>

int main() {
    int pin = 17;
    int state = 0;
    int value;

    gpioInitialise();

    gpioSetMode(pin, PI_INPUT);

    while(1) {
        value = gpioRead(pin);
        if (value == 1 && state == 0) {
            state = 1;
            printf("on\n");
        } else if (value == 0 && state == 1) {
            state = 0;
            printf("off\n");
        }
        gpioDelay(1000);
    }

    gpioTerminate();

    return 0;

}
