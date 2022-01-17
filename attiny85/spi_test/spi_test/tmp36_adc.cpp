#include <avr/io.h>
#include <inttypes.h>

#include "prj_hardware.h"

// Determine the prescalar to use for ADC conversion. The datasheet says 50 KHz to 200 Khz gets maximum resolution.
#if F_CPU == 1000000
#define TMP36_PRESCALAR 3
#elif F_CPU == 2000000
#define TMP36_PRESCALAR 4
#elif F_CPU == 4000000
#define TMP36_PRESCALAR 5
#elif F_CPU == 8000000
#define TMP36_PRESCALAR 6
#else
// Assume 1MHz CPU speed.
#define TMP36_PRESCALAR 3 
#endif

namespace scottz0r { namespace tmp36 {
    static constexpr int16_t no_temperature = 0x8000; // Min value of int16.
    
    static int16_t s_last_measure = no_temperature;

    static int16_t adc_to_c_float(uint16_t adc);
    
    void init()
    {
        // Set ADC Multiplexer selection register to use configured pin and use VCC as reference.
        // This will be single input mode.
        ADMUX = TMP36_PIN_MUX;
        
        // Enable ADC and set prescalar (this is based on CPU speed).
        ADCSRA = (1 << ADEN) | TMP36_PRESCALAR;
    }
    
    void process()
    {
        // If the Interrupt flag is set, then the ADC conversion is complete.
        if (ADCSRA & (1 << ADIF))
        {            
            s_last_measure = adc_to_c_float(ADC);
            
            // Clear interrupt flag by writing a 1.
            ADCSRA |= (1 << ADIF);
        }
    }
    
    void start()
    {
        // Check if converting. If not, enable flag. Otherwise, do nothing.
        int is_converting = ADCSRA & (1 << ADSC);
        
        if (!is_converting)
        {
            // Set the start bit and clear the interrupt flag (1 = clear).
            ADCSRA |= (1 << ADSC) | (1 << ADIF);
        }
    }
        
    void clear()
    {
        s_last_measure = no_temperature;
    }
    
    bool last_measurement(int16_t& value)
    {
        // If the high bit is set, then there is no valid value.
        if (s_last_measure == no_temperature)
        {
            return false;
        }
        
        value = s_last_measure;
        return true;
    }

    // Note: This uses floating point math, which adds a lot of code.    
    static int16_t adc_to_c_float(uint16_t adc)
    {
        float voltage = adc * (5000.0 / 1024.0);
        float tempr = (voltage - 500) / 10.0;
        
        int16_t temp_100ths = (int16_t)(tempr * 100.0);
        return temp_100ths;
    }
}}
