#include <LiquidCrystal.h>
const int rs = 4, en = 5, d4 = 6, d5 = 7, d6 = 8, d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
byte statusLed    = 13;

byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin       = 3;


float calibrationFactor = 7.5;

volatile byte pulseCount;  

float flowRate;
unsigned int flowML;
unsigned long totalML;

unsigned long time;

void setup()
{
  
  
  // Set up the status LED line as an output
  pinMode(statusLed, OUTPUT);
  digitalWrite(statusLed, HIGH); 
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);
  
  pulseCount        = 0;
  flowRate          = 0.0;
  flowML   = 0;
  totalML  = 0;
  time           = 0;
  
  // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
}

/**
* Main program loop
*/
void loop()
{
  
  if((millis() - time) > 1000)    // Only process counters once per second
  { 
    
    detachInterrupt(sensorInterrupt);
    
    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - time)) * pulseCount) / calibrationFactor;
    
    // Note the time this processing pass was executed. Note that because we've
    // disabled interrupts the millis() function won't actually be incrementing right
    // at this point, but it will still return the value it was set to just before
    // interrupts went away.
    time = millis();
    
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowML = (flowRate / 60) * 1000;
    
    // Add the millilitres passed in this second to the cumulative total
    totalML += flowML;
    
    unsigned int frac;
    
    lcd.begin(16, 2);
    lcd.print(" Flow Rate");
    lcd.setCursor(1, 1);
    lcd.print(flowRate);
    lcd.setCursor(6, 1);
    lcd.print("L/Min");
    
    
    
    // Reset the pulse counter so we can start incrementing again
    pulseCount = 0;
    
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
}
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}