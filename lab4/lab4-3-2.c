#include <stdio.h>
#include <pigpio.h>

int main() {
    int led1 = 12;
    int step = 10000;
    int delay = 10000;
    int freq = 10000;
    int duty = 0;

    gpioInitialise();

    while(1) {
        duty += step;
        if (duty >= 1000000 || duty <= 0) {
            step = -step;
        }

        gpioHardwarePWM(led1, freq, duty);

        gpioDelay(delay);
    }

    gpioTerminate();

    return 0;
}