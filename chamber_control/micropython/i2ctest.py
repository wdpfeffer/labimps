from machine import Pin, SoftI2C

i2c = SoftI2C(scl=Pin(3), sda=Pin(2), freq=100_000)

print(i2c.scan())