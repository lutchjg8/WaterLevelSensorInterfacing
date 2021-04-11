#include <avr/io.h>
#include <util/delay.h>

/* This is the Design By Contract macros.*/
#define DBC // Can turn off these macros by commenting out this line
#ifdef DBC
/* needs to be at main since we are going to use Pin13 as our LED to warn us on assert fails */
#define DBC_SETUP()                                                    \
/* turn on Pin 13 as we will use to indicate assertion/error failed */ \
DDRB |= _BV(DDB5); 

#define PRE_CONDITION_DBC(eval_expression, time_blink_delay) \
while (!(eval_expression))                                   \
{                                                            \
  PORTB |= _BV(PORTB5);                                      \
  my_delay_ms(time_blink_delay);                             \
  PORTB &= ~_BV(PORTB5);                                     \
  my_delay_ms(time_blink_delay);                             \
}

#define POST_CONDITION_DBC(eval_expression, time_blink_delay) \
while (!(eval_expression))                                    \
{                                                             \
  PORTB |= _BV(PORTB5);                                       \
  my_delay_ms(time_blink_delay);                              \
  PORTB &= ~_BV(PORTB5);                                      \
  /* half the delay off on post condition */                  \
  my_delay_ms(time_blink_delay/2);                            \
}
#else
/* These are empty for when turned off */
#define DBC_SETUP() {}
#define PRE_CONDITION(eval_expression, time_blink_delay) {}
#define POST_CONDITION(eval_expression, time_blink_delay) {}
#endif

/* C Prototypes of functions */
void my_delay_ms( unsigned int t);
short readSensor(int a);
int lowerThresh = 100;
int upperThresh = 450;

int main (void) 
{
  DBC_SETUP();
  DDRD |= _BV(DDD2);
  DDRD |= _BV(DDD3);
  DDRD |= _BV(DDD4);
  DDRD |= _BV(DDD7);

  while(1) 
  {
    PORTD |= _BV(PORTD7);
    int val = readSensor(PC0);
    if (val < lowerThresh)
    {
      PORTD |= _BV(PORTD2);
    }
    else
    {
      PORTD |= ~_BV(PORTD2);
    }
    if(val > lowerThresh & val < upperThresh)
    {
      PORTD |= _BV(PORTD3);
    }
    else
    {
      PORTD |= ~_BV(PORTD3);
    }
    if(val > upperThresh)
    {
      PORTD |= _BV(PORTD4);
    }
    else
    {
      PORTD |= ~_BV(PORTD4);
    }
  }
}
short readSensor(int a)
{
  int b = a;
  return b;
}


/* 
* Handles larger delays the _delay_ms can't do by itself (not sure how accurate)  
* Note no DBC as this function is used in the DBC !!! 
*
* borrowed from : https://www.avrfreaks.net/forum/delayms-problem 
* */
void my_delay_ms(unsigned int t) 
{
  unsigned int i;
  
  for (i=0; i<(t/10); i++) 
  {
    _delay_ms(10);
  }
  if (t % 10) {
    _delay_ms(10);
  }
}
