class Power():
    from machine import Pin, PWM, ADC
    
    max = 65534
    
    def __init__(self, pp=None, ap=None):
        if pp != None:
            self.setPWM(pp)
            
        if ap != None:
            self.setADC(ap)
    
    def setPWM(self, pin: int):
        self.powerpin = self.PWM(self.Pin(pin))
        
    def setADC(self, pin: int):
        self.adcpin = self.ADC(self.Pin(pin))
        
    def read(self):
        return self.adcpin.read_u16()
    
    def readVolts(self):
        frac = self.read()/self.max
        return 3.3*frac
    
    def setPower(self, level: int):
        frac = level/100
        plevel = int(frac*self.max)
        self.powerpin.duty_u16(plevel)