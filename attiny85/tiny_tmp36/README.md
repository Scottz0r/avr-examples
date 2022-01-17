# Tiny TMP36

Read a TMP36 with the ADC and send reading via SPI.

## Hardware

* ATTiny85. 1MHz clock
* TMP36

## Wiring

|Pin|Direction|Desc|
|---|---------|----|
|2|In|TMP36 (ADC)|
|3|Out|Ready Indicator (1 = ready)|
|5|In|Data In (DI)|
|6|Out|Data Out (DO)|
|7|In|Clock (SCK)|
