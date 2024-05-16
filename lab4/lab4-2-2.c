#include <stdio.h>
#include <pigpio.h>

int main() {
    int pin = 18;
    int switch_pin = 24;
    int flag = 0;

    if (gpioInitialise() < 0) {
        printf("Failed to initial\n");
        return 1;
    }

    gpioSetMode(pin, PI_OUTPUT);
    gpioSetMode(switch_pin, PI_INPUT);

    while(1) {
        if (gpioRead(switch_pin) == 0) {
        // 스위치를 눌렀을 때 LED 상태를 토글
            flag = !flag;
            gpioWrite(pin, flag);

        // 스위치가 떨어질 때까지 기다림 (바운스 방지)
            while (gpioRead(switch_pin) == 0) {
                gpioDelay(10000); // 10ms
            }
        }
    }

    gpioTerminate();

    return 0;
}