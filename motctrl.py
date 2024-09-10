from machine import Pin, PWM
import uasyncio
import time

class ctrl():
    pwm = None
    trig = 0
    start = 0
    waiting = false

    def __init__(self, pin=5, fq=300, dty=0):
        self.pwm = PWM(Pin(pin), freq=fq, duty=dty)
        
    def power(self, p=0):
        #take percent value and converts it to duty
        print(p)
        if (p>=0) and (p<=100):
            f= int(p/100.0*1023)
            print('Power: {}%'.format(p), 1)
            self.pwm.duty(f)
        else:
            self.pwm.duty(0)
            print('Power: 0%'.format(p), 1)
            
    def rampup(self):
        import time
        for i in range(70, 101):
            self.power(i)
            time.sleep(0.2)
    
    def rampdown(self):
        for i in range(100, 70):
            self.power(i)
            time.sleep(0.2)
    
    def shutdown(self, t):
        ct=0
        tsec=t*60
        go = True
        while go:
            if ct > tsec:
                self.power(0)
                go = False
            ct += 1
            time.sleep(1)
            
    async def runtach(self, trig):
        delta = (trig-self.start)/60
        try:
            rpm = 0.5/delta
            print('{}'.format(rpm))
        except:
            pass
        self.start = time.time()
        self.waiting = false
        
                
    async def tach(self):
        from machine import ADC
        ac = ADC(0)
        self.start = time.time()
        
        while True:
            lv = ac.read()
            if lv < 500 and not self.waiting:
                # get rpm
                waiting = true
                rpm_task = uasyncio.create_task(self.runtach(time.time()))
                await rpm_task
                
        
                
            
        