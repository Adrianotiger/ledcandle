/*
 * Project: LED Candle
 * Author: Adriano Petrucci
 * Date: Feb. 2018
 * Description: Will flicker a LED for 5-10 minutes and then power off.
 *              To save energy, the uC will go to sleep for 4 seconds after each flicker
 * Version: 1.0
 * 
 * References:
 * Flicker code: https://github.com/aShure/arduino_sketches/tree/master/Candle
 */


#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

  // Define PINs
uint8_t pinPower  = 2;    // Used to power the ATTiny
uint8_t pinLed    = 1;    // LED
uint8_t pinInt    = 0;    // Push button interrupt
uint8_t pinInp1   = 3;    // Switch 1
uint8_t pinInp2   = 4;    // Switch 2

  // the start of the flicker (low)
uint8_t flicker_low_min = 190;
uint8_t flicker_low_max = 230;
  // the end value of the flicker (high)
uint8_t flicker_high_min = 220;
uint8_t flicker_high_max = 255;
  // delay between each low-high-low cycle
  // low->high |flicker_hold| high->low
static uint16_t flicker_hold_min = 50; // milliseconds
static uint16_t flicker_hold_max = 90; // milliseconds
  // delay after each low-high-low cycle
  // low->high->low |flicker_pause| low->high...
static uint16_t flicker_pause_min = 100; // milliseconds
static uint16_t flicker_pause_max = 200;  // milliseconds
  // delay low to high and high to low cycle
static uint16_t flicker_speed_min = 500; // microseconds
static uint16_t flicker_speed_max = 1000; // microseconds

uint8_t flicker_random_low_start = 0;
uint8_t flicker_random_low_end = 0;
uint8_t flicker_random_high = 0;
uint16_t flicker_random_speed_start = 0;
uint16_t flicker_random_speed_end = 0;

  // How long the module should be powered on
volatile uint32_t autoPowerOffTime = 1000L * 60 * 10;  // 10 minutes
  // millis() does not work on this example, as we don't have an external timer and we go to sleep every 4-8 seconds
volatile uint32_t onTime = 0L; 
  // used to add time to onTime
volatile uint32_t tempMillis = 0L;
  // push button pressed (power off candle)
volatile bool powerOffCandle = false;

void setup() 
{
  power_adc_disable();            // Don't need an Analog Digital Converter
  power_usi_disable();            // Don't need the Universal Serial Interface
  power_timer1_disable();         // Don't need the Timer1

  pinMode(pinPower, OUTPUT);      // Auto power the module
  digitalWrite(pinPower, HIGH);
  pinMode(pinLed, OUTPUT);        // LED pin (PWM)

  pinMode(pinInt, INPUT_PULLUP);  // Interrupt
  pinMode(pinInp1, INPUT_PULLUP); // Switch 1
  pinMode(pinInp2, INPUT_PULLUP); // Switch 2

  GIMSK = 0b00100000;             // turns on pin change interrupts
  PCMSK = (1 << pinInt);          // turn on interrupts on pins PB0
  sei();                          // enables interrupts

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here (SLEEP_MODE_IDLE if PWM must still be active) - SLEEP_MODE_PWR_DOWN -> SLEEP_MODE_STANDBY -> SLEEP_MODE_PWR_SAVE

  setup_watchdog(WDTO_4S);        // approximately 4 seconds sleep

  powerOffCandle = false;         // true if candle need to power off
  autoPowerOffTime = 1000L * 20;  // 20 seconds

    // nice power on effect
  for(uint8_t k=0;k<10;k++)
  {
    analogWrite(pinLed, (k%2)==0?k*k*2:k*k);
    delay(50);
  }
}

void loop() 
{
  tempMillis = millis();  // calculate the millis used for the flicker animation
  
    // random time for low
  flicker_random_low_start = random(flicker_low_min, flicker_low_max);
  flicker_random_low_end = random(flicker_low_min, flicker_low_max);
   
    // random time for high
  flicker_random_high = random(flicker_high_min, flicker_high_max);
   
    // random time for speed
  flicker_random_speed_start = random(flicker_speed_min, flicker_speed_max);
  flicker_random_speed_end = random(flicker_speed_min, flicker_speed_max);

     // high -> low
  for (int i = flicker_random_high; i>=flicker_random_low_end; i--) 
  {
    analogWrite(pinLed, i);
    delayMicroseconds(flicker_random_speed_end);
  }
     
    // hold
  delay(random(flicker_hold_min, flicker_hold_max));

    // low -> high
  for (int i = flicker_random_low_start; i<flicker_random_high; i++) 
  {
    analogWrite(pinLed, i);
    delayMicroseconds(flicker_random_speed_start);
  }

  if(random(0,4) == 0 && onTime < autoPowerOffTime)
  {
    onTime += millis() - tempMillis;

    for (int i = flicker_random_high;i<255; i++) 
    {
      analogWrite(pinLed, i);
      delayMicroseconds(flicker_random_speed_start);
    }
    //analogWrite(pinLed, 255);
    do
    {
      system_sleep();
      onTime += 3700;
    } while(random(0,2) == 0);
  }
  else
  {
      // hold
    delay(random(flicker_hold_min, flicker_hold_max));
    onTime += millis() - tempMillis;
  }

    // If timeout or button was pressed
  if(onTime > autoPowerOffTime || powerOffCandle) 
  {
    if(powerOffCandle || flicker_low_min == 0)
    {
      delay(300);
      for(int i=30;i>0;i--)
      {
        analogWrite(pinLed, i);
        delay(30 + (30-i));
      }
      digitalWrite(pinPower, LOW);
      delay(500);
      analogWrite(pinLed, 255);
      delay(5000);
    }
    else if(flicker_low_min > 50) 
    {
      flicker_low_min -= 5;
      flicker_low_max -= 5;
      flicker_high_min -= 4;
      flicker_high_max -= 4;
      if(flicker_speed_min > 100)
      {
        flicker_speed_min -= 50;
        flicker_speed_max -= 50;
      }
    }
    else if(flicker_low_min > 30) 
    {
      flicker_low_min -= 3;
      flicker_low_max = flicker_low_min + flicker_low_max / 5;
      flicker_high_min = flicker_low_min + 20;
      flicker_high_max = flicker_low_min + 30;
      flicker_speed_min = 50;
      flicker_speed_min = 200;
    }
    else
    {
      powerOffCandle = true;
    }
  }
}

  // set system into the sleep state 
  // system wakes up when wtchdog is timed out
void system_sleep() 
{
  sleep_enable();
  sleep_mode();                          // System actually sleeps here
  sleep_disable();                       // System continues execution here when watchdog timed out 
}

  // 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
  // 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(uint8_t ii) 
{ 
  byte bb;
  uint8_t ww;
  if (ii > 9 ) ii=9;
  bb = ii & 7;
  if (ii > 7) bb |= (1<<5);
  bb |= (1<<WDCE);
  ww = bb;
  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCR = bb;
  WDTCR |= _BV(WDIE);
}


  // Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect) 
{
  // Coming out from sleep
}

  // Interrupt 0 detected
ISR(PCINT0_vect) 
{
  if(onTime > 100)  // Only after 100ms (debouncing)
  {
    if(digitalRead(pinInt) == LOW)
      powerOffCandle = true;
  }
}

