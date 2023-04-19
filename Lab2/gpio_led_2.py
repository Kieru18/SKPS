import gpio4
import time
from itertools import repeat
import math

def PWM(gpio, frequency, duty_cycle):
    now = time.time()
    
    if isinstance(frequency, int):
        frequency = repeat(frequency)

    if isinstance(duty_cycle, (int, float)):
        duty_cycle = repeat(duty_cycle)

    for p, dc in zip(frequency, duty_cycle):
        cycle = 1/p
        on = cycle * dc
        off = cycle - on

        if on:
            gpio.value = 1  
            # print(f'HIGH, {now}')
            start = time.time()
            now = time.sleep(on)
            now = start

        if off:
            gpio.value = 0
            # print(f'LOW, {now}')
            start = time.time()
            now = time.sleep(on)
            now = start
            

   


def main():
    gpio27 = gpio4.SysfsGPIO(27)
    gpio27.export = True
    gpio27.direction = 'out'
    gpio27.value = 0

    PWM(gpio27, 12, 1024)
    time.sleep(3)

    gpio27.export = False

if __name__ == "__main__":
    main()
