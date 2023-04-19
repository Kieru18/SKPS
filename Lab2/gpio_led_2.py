import gpio4
import time
from itertools import repeat
import math

def PWM(gpio, frequency, duty_cycle, cycles, delta):
    gpio.export = True
    gpio.direction = 'out'
    gpio.value = 0

    cycle = 1 / frequency
    on = cycle * duty_cycle
    off = cycle - on

    flag = True

    for _ in range(cycles):
        gpio.value = 1
        time.sleep(on)
        gpio.value = 0
        time.sleep(off)

        if duty_cycle == 100:
            flag = False
        
        if duty_cycle == 0:
            flag = True
        
        if flag:
            duty_cycle += delta
        else:
            duty_cycle -= delta

        cycle = 1 / frequency
        on = cycle * duty_cycle
        off = cycle - on

    gpio.export = False


def main():
    PWM(27, 50, 0, 2, 300)

if __name__ == "__main__":
    main()
