from machine import Pin
import time

lights_button = Pin(27, Pin.IN)
lights_output = Pin(0, Pin.OUT)


lights_button_state = 0
lights_button_reading = 0
lights_state = 0
last_lights_button_state = 0
last_debounce_timer_lights = 0

debounce_delay = 50 #milliseconds

while True:
    
    #get new values from switches
    lights_button_reading = lights_button.value()
    
    # check to see if button pressed
    if lights_button_reading != last_lights_button_state:
        
        #reset the debounce timer
        last_debounce_timer_lights = time.ticks_ms()
    
    if time.ticks_diff(time.ticks_ms(), last_debounce_timer_lights) > debounce_delay:
        
        #reading has been there awhile
        #see if button state has changed
        if lights_button_reading != lights_button_state:
            lights_button_state = lights_button_reading
            
            #toggle the output
            if lights_button_state == 1:
                lights_state = not(lights_state)
            
    
    lights_output.value(lights_state)
    last_lights_button_state = lights_button_reading