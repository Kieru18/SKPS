import gpio4
import time

gpio27 = gpio4.SysfsGPIO(27)
gpio27.export = True
gpio27.direction = 'out'
gpio27.value = 0

for i in range(10):
    gpio27.value = 1
    time.sleep(0.5)
    gpio27.value = 0
    time.sleep(0.5)

gpio27.export = False