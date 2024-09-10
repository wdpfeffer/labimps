from machine import Pin, I2C
import time
import dht
import ssd1306


i2c = I2C(0, sda=Pin(12), scl=Pin(13))
display = ssd1306.SSD1306_I2C(128, 64, i2c)

debounce_delay = 50 #milliseconds
temphum_delay = 1000 #milliseconds

class buttonstate:
    
    button_state = 0
    button_reading = 0
    state = 0
    last_button_state = 0
    last_debounce_timer = 0
  
    def __init__(self, inpin:Pin , outpin:Pin):
        self.inpin = inpin
        self.outpin = outpin
    
    def update_button_status(self):
            
        #get new values from switches
        self.button_reading = self.inpin.value()
        
        # check to see if button pressed
        if self.button_reading != self.last_button_state:
            
            #reset the debounce timer
            self.last_debounce_timer = time.ticks_ms()
        
        if time.ticks_diff(time.ticks_ms(), self.last_debounce_timer) > debounce_delay:
            
            #reading has been there awhile
            #see if button state has changed
            if self.button_reading != self.button_state:
                self.button_state = self.button_reading
                
                #toggle the output
                if self.button_state == 1:
                    self.state = not(self.state)
                
        
        self.outpin.value(self.state)
        self.last_button_state = self.button_reading
      
class temphum:
    last_report_timer=0
    
    def __init__(self, pin:Pin, display):
        self.pin = pin
        self.display = display
        self.tdht = dht.DHT22(pin)
        
    def report(self):
        if time.ticks_diff(time.ticks_ms(), self.last_report_timer) > temphum_delay:
            self.tdht.measure()
            temp = self.tdht.temperature()
            hum = self.tdht.humidity()
            self.display.fill(0)
            self.display.text('Temp {}'.format(temp), 0, 0)
            self.display.text('Humid {}'.format(hum), 0, 16)
            self.display.show()
            #update the timer
            self.last_report_timer = time.ticks_ms()
        


lights_button = Pin(27, Pin.IN)
lights_output = Pin(0, Pin.OUT)

lights = buttonstate(lights_button, lights_output)
thprobe = temphum(Pin(16), display)

while True:
    
    lights.update_button_status()
    thprobe.report()


