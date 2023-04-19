import time
from itertools import repeat

def PWM(period, duty_cycle):
    now = time.time()
    
    if isinstance(period, int):
        period = repeat(period)

    if isinstance(duty_cycle, float):
        duty_cycle = repeat(duty_cycle)

    for p, dc in zip(period, duty_cycle):
        cycle = 1/p
        on = cycle * dc
        off = cycle - on

        if on:
            print(f'HIGH, {now}')
            start = time.time()
            now = time.sleep(on)
            now = start

        if off:
            print(f'LOW, {now}')
            start = time.time()
            now = time.sleep(on)
            now = start
            
        

if __name__ == '__main__':
    PWM(5, [0.01, 0.1, 0.5, 0.9, 0.99])
    #PWM([1, 2, 3, 4, 5, 4, 3, 2, 1], 0.5)

