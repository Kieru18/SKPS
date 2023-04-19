import gpio4
import time

frequencies = [261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25, 587.33, 659.25, 698.46, 783.99, 880.00, 987.77, 1046.50]

def PWM(gpio, frequency, duty_cycle, cycles):
    gpio.export = True
    gpio.direction = 'out'
    gpio.value = 0

    cycle = 1 / frequency
    on = cycle * duty_cycle
    off = cycle - on


    for _ in range(cycles):
        gpio.value = 1
        time.sleep(on)
        gpio.value = 0
        time.sleep(off)

        cycle = 1 / frequency
        on = cycle * duty_cycle
        off = cycle - on

    gpio.export = False

if __name__ == "__main__":
    buzzer = gpio4.SysfsGPIO(18)

    for frequency in frequencies:
        PWM(buzzer, frequency, 0, 1000)
