# SPI Test

Uses the Universal Serial Interface (USI) to do non-blocking SPI communication.

USI and ADC are non-blocking.

Timer0 is used for a millisecond counter.

This project contains a small example of a user interface via a LED status light. This
single light overcomes the difficulty of outputting information with a limited number of
pins.

## Hardware

* ATTiny85. 1MHz clock
* TMP36
* LED with 330 Ohm resistor.

## Wiring

|Pin|Direction|Desc|
|---|---------|----|
|2|In|TMP36 (ADC)|
|3|Out|LED Status|
|5|In|Data In (DI)|
|6|Out|Data Out (DO)|
|7|In|Clock (SCK)|
