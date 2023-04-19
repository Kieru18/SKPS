import gpio4
import time
from itertools import repeat
import math

def PWM(gpio, frequency, duty_cycle):
    for ii in range(frequency):
        gpio.value = 1
        time.sleep(duty_cycle/10000)
        gpio.value = 0
        time.sleep(0.01 - duty_cycle/10000)
   


def main():
    gpio27 = gpio4.SysfsGPIO(27)
    gpio27.export = True
    gpio27.direction = 'out'
    gpio27.value = 0

    for ii in range(100):
        s = (math.sin(ii/10)+1)/2 * 100
        PWM(gpio27, 10, s)

    gpio27.export = False

if __name__ == "__main__":
    main()
