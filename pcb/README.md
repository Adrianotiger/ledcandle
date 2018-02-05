![alt text](https://github.com/Adrianotiger/ledcandle/blob/master/pcb/Led_candle_schema.png "Led Candle Schema, version 1.3")

# Schematics

## How it works
The print is divided in 2 parts. A power latch and the timer control.
Once you press the push bush button, the µC will be powered and will auto power itself until the timeout is over or if the push button is pressed again.
There is a dipswitch that can be used to set some variables inside the code or to program the attiny.

## components / BOM

| Qty | Component | Price ($) | Description |
|-----|-----------|-------|-------------|
|  1  | V1, Battery holder | 0.06$ | A simple 2032 battery with 230mAh, will power the module for 25-40 hours |
|  1  | Q2, BSS84 | 0.02$ | P-channel MOSFET. Once the switch is pressed, the FET will power on the entire module (VCC = battery voltage) |
|  1  | Q3, BSS138 | 0.02$ | N-channel MOSFET. The µC will activate it (high on gate). Once the µC will set the gate to low, the P-MOSFET will close (VCC = 0V) |
|  1  | K1, Push button | 0.02$ | To power on (and power off) the module. In my project I use a bridge near the LED |
|  1  | LED | 0.01$ | A bright yellow/orange/ambra LED. Something like 8000 mcd at 20mA. Search a LED that works with 1-2V, not 4-5V! |
|  1  | R2,R3,R4 100kΩ | 0.01$ | 2 pullups and 1 pulldown resistors, 10k would also works, but R3 should be as high as possible |
|  1  | R1 150Ω | 0.01$ | LED resistor, choose the resistor for your LED. You should not power the LED with more than 20mA - 5mA in my project |
|  1  | ISP1 Conn. | 0.01$ | A connector (if you want) to program the ATTiny45V, holes are enough small to put the programming connector directly in the PCB |
|  1  | U2 DipSwitch | 0.15$ | Not necessary, you can bridge the pins directly, as this is an expensive component |
|  1  | D1 BAS40-05 | 0.02$ | Dual schottky diode. You need this if you want detect a second press over the µC.* |
|  1  | U1 ATTiny45V | 1.40$ | AT Tiny45 V, is like the AT Tiny45 but you can power it from 1.8V to 5.5V (the normal one only from 2.7 to 5.5V)** |
|  ∞  | 2032 Battery | 0.50$ | A simple 2032 battery, you can get them from 0.10$ up to 1$. Don't know if there is a big difference) |
|  1  | PCB print | 0.40$ | If you buy 5, you have to pay 2$ on easyeda.com |

## Troubles
***Schottky (D1):***

The main problem there is the reverse current. The module will consume even if it should be powered off. *BAS40-05* has only 10nA reverse current, in this way you can detect the switch wihtout loose too much current - you will loose 1% of your battery capacity each year. 
Another way is to remove the connection between µC and switch (cut the track on the PCB between the 2 pads). You will not be able to power off the led anymore but the module will power on also after 10 years.

***ATTiny45V (U1):***

Pratically impossible to find on cheaps shops, this ATTiny is made to works on low voltages. The normal ATTiny works with at least 2.7V but the 3V battery will go down up to 2.0V when discharging. So the only way is to use this V-type ATTiny. The rest is perfect:
4kB program memory, 256 bytes of RAM and even 256 bytes of ROM with a clock that works up to 10MHz (we use only 2kB and 50 bytes of RAM and need a clock of 1MHz, this will reduce the consumption a little bit).

***MOSFETS:***

BSS84 and BSS138, maybe not the best ones but the cheapest on the market. Perfect for this work.

***Battery Holder***

The one used on this project is really big! There are smaller holders, SMD holders for example. If you want a flat module, you have to choose another type.
