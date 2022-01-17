#pragma once

#include <avr/io.h>

// Processor speed
#define F_CPU 1000000

// Voltage the project will be running on
#define VOLTAGE 5

// Output LED on Pin 3
#define LED_PIN (PB4)
#define LED_PORT (PORTB)
#define LED_DDR (DDRB)

// TMP36 on ADC3, Pin 2
#define TMP36_PIN_MUX 3
