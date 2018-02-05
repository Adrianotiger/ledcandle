# Schematics

## How it works
The print is divided in 2 parts. A power latch and the timer control.
Once you press the push bush button, the uC will be powered and will auto power itself until the timeout is over or if the push button is pressed again.
There is a dipswitch that can be used to set some variables inside the code or to program the attiny.

## components / BOM

| Qty | Component | Price ($) | Description |
|-----|-----------|-------|-------------|
|  1  | V1, Battery holder | 0.06$ | A simple 2032 battery with 230mAh, will power the module for 25-40 hours |
|  1  | Q2, BSS84 | 0.02$ | P-channel MOSFET. Once the switch is pressed, the FET will power on the entire module (VCC = battery voltage) |
|  1  | Q3, BSS138 | 0.02$ | N-channel MOSFET. The uC will activate it (high on gate). Once the uC will set the gate to low, the P-MOSFET will close (VCC = 0V) |
|  1  | K1, Push button | 0.02$ | To power on (and power off) the module. In my project I use a bridge near the LED |
|  1  | LED | 0.01$ | A bright yellow/orange/ambra LED. Something like 8000 mcd at 20mA. Search a LED that works with 1-2V, not 4-5V! |
|  1  | R2,R3,R4 100k | 0.01$ | 2 pullups and 1 pulldown resistors, 10k would also works, but R3 should be as high as possible |
|  1  | R1 | 0.01$ | LED resistor, choose the resistor for your LED. You should not power the LED with more than 20mA - 5mA in my project |
|  1  | ISP1 Conn. | 0.01$ | A connector (if you want) to program the ATTiny45V, holes are enough small to put the programming connector directly in the PCB |
|  1  | U2 DipSwitch | 0.15$ | Not necessary, you can bridge the pins directly, as this is an expensive component |
|  1  | D1 BAS40-05 | 0.02$ | Dual schottky diode. You need this if you want detect a second press over the uC.* |
|  1  | U1 ATTiny45V | 1.40$ | AT Tiny45 V, is like the AT Tiny45 but you can power it from 1.8V to 5.5V (the normal one only from 2.7 to 5.5V)** |

## Troubles
***Schottky (D1):***
The main problem there is the reverse current. The module will consume even if it should be powered off. BAS40-05 has only 10nA, in this way you can detect the switch wihtout loose too much current. Another way is to remove the connection between uC and switch. You will not be able to power off the led anymore but the module will power on also after 10 years.
