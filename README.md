# Frequency Meter

Frequency by its definition is events occurred per second. This project is a frequency meter that uses the timer/counter feature of 89C51 microcontroller to count the pulses in each second of operation and diplays the output on the LCD. The range of this frequency meter is from 1Hz to 125KHz. Frequency meter v1 uses timer1 of the microcontroller to generate time delay and timer0 as a counter to count the pulses. The output can be fine tuned by modifying the loop valye at line number 54 of "Frequency Counter v1.c" code.

Frequency Counter v2 take cares of automatic scaling and unit selection while it measures the frequency. The input in both cases is given to T0 pin of the microcontroller.

The youtube video can be accessed through the following link:

https://www.youtube.com/watch?v=nxLB2I8NMTQ