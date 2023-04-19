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
        cycle = 1000/p
        on = cycle * dc

        off = cycle - on
        if off < 0:
            off = 0
        

        if on:
            gpio.value = 1  
            # print(f'HIGH, {now}')
            start = time.time()
            now = time.sleep(on/1000)
            now = start

        if off:
            gpio.value = 0
            # print(f'LOW, {now}')
            start = time.time()
            now = time.sleep(off/1000)
            now = start
            

   


def main():
    gpio27 = gpio4.SysfsGPIO(27)
    gpio27.export = True
    gpio27.direction = 'out'
    gpio27.value = 0

    PWM(gpio27, 12, range(0, 1025))

    gpio27.export = False

if __name__ == "__main__":
    main()
