# AVR Examples

This project contains various test/demo projects made with AVR Studio.

Projects are organized by microcontroller.

## Programming

An ISP programmer is needed to flash programs. The easiest way is to use an Arduino with the `ArduinoISP` sketch. There are many examples of setting this up elsewhere.

The Arduino IDE will install the avrdude executable, which can upload sketches. I use a helper PowerShell script to assist in this. The easiest way to find out which arguments to use is to enable Verpose output (Preferences) to see how Arduino is calling the program. An example for the `spi_test` for the ATTiny85 is below.

```PowerShell
$avrdude = "C:\<Your User Name>\Scott\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino18/bin/avrdude"
$config = "C:\<Your User Name>\Scott\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino18/etc/avrdude.conf"

$hex_file = "spi_test\Release\spi_test.hex"

& $avrdude "-C$config" -v -pattiny85 -cstk500v1 -PCOM5 -b19200 "-Uflash:w:${hex_file}:i"
```
