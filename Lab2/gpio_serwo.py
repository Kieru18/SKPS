from time import sleep
import gpio4

def pwm(duty_cycle: float=0, frequency: float=0.1, cycles: int=1000) -> None:
    gpio = gpio4.SysfsGPIO(18)
    gpio.export = True
    gpio.direction = 'out'
    gpio.value = 0

    cycle = 1 / frequency
    on = cycle * (duty_cycle / 100)
    off = cycle - on

    for _ in range(cycles):
        gpio.value = 1
        sleep(on)
        gpio.value = 0
        sleep(off)

        cycle = frequency**(-1)
        on = cycle * (duty_cycle * 0.01)
        off = cycle - on

    gpio.export = False

def main():
    for i in range(1000):
        pwm(duty_cycle=i/1000)

if __name__ == '__main__':
    main()

