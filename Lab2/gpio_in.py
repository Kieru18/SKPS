import gpio4

led = gpio4.SysfsGPIO(27)
led.export = True
led.direction = 'out'

button = gpio4.SysfsGPIO(25)
button.export = True
button.direction = 'in'

while True:
    try:
        if button.value == 1:
            led.value = 1
        else:
            led.value = 0
    except KeyboardInterrupt:
        led.export = False
        button.export = False
        break