import gpio4
import time
from itertools import repeat
import math

def PWM(gpio, frequency, duty_cycle):
    oscillation = 1 / frequency
    high_duration = oscillation * (duty_cycle / 100)
    low_duration = oscillation - high_duration

    flag=True
    
    for _ in range(cycles):
        start = time()
        
        gpio.value = 1
        sleep(high_duration)
        gpio.value = 0
        sleep(low_duration)        

        if (duty_cycle == 100):
            flag=False
        if (duty_cycle == 0):
            flag=True
	
        if flag:
           duty_cycle += duty_cycle_change
        else:
           duty_cycle -= duty_cycle_change        
        
        oscillation = 1 / frequency
        high_duration = oscillation * (duty_cycle / 100)
        low_duration = oscillation - high_duration
    


def main():
    gpio27 = gpio4.SysfsGPIO(27)
    gpio27.export = True
    gpio27.direction = 'out'
    gpio27.value = 0

    PWM(gpio27, 12, range(0, 1025))

    gpio27.export = False

if __name__ == "__main__":
    main()
