from machine import Pin, I2C
from ssd1306 import SSD1306_I2C


i2c = I2C(0, scl=Pin(17), sda=Pin(16)) 
print(i2c.scan())

#oled = SSD1306_I2C(128,32,i2c)
# 
# pin = machine.Pin(16, machine.Pin.OUT)
# pin.value(0) #set GPIO16 low to reset OLED
# pin.value(1) #while OLED is running, must set GPIO16 in high
# 
# oled_width = 128
# oled_height = 64
# oled = SSD1306_I2C(oled_width, oled_height, i2c)
# 
# oled.fill(0)
# oled.text('Hello, World 1!', 0, 0)
# oled.text('Hello, World 2!', 0, 10)
# oled.text('Hello, World 3!', 0, 20)
# oled.show()