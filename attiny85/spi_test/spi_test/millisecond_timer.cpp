#include "prj_hardware.h"
#include "millisecond_timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

namespace scottz0r { namespace ms {
    static volatile timespan_t timer1_millis = 0;
    
    ISR (TIMER0_COMPA_vect)
    {
        ++timer1_millis;
    }
    
    void init()
    {
        // Using this as an example: https://github.com/monoclecat/avr-millis-function/blob/master/millis.c
            
        static_assert(((F_CPU / 1000) / 8) < 256, "Exceeds 8-bit timer");
            
        // This formula can be broken down into:
        // - Want this to occur 1,000 times a second (for ms), so get how many clock cycles is in a millisecond.
        // - Need to use a prescalar to scale the number so it fits into an 8-bit register.
        //     8 is a nice divisor for 1MHZ because it is an even number.
        //
        // For a 1MHz clock, this comes out to 125. So, when the timer ticks 125 times (at 'intervals' of 8), 1ms has passed.
        constexpr uint8_t ctc_match = (F_CPU / 1000) / 8;
            
        // WGM01 = CTC
        TCCR0A |= (1 << WGM01);
            
        // CS01 = 8 prescalar
        TCCR0B |= (1 << CS01);
            
        // Set the output control register, which will control when Timer 0 interrupts.
        // ATTiny85 has 8-bit register.
        OCR0A = ctc_match;
            
        // Enable the CTC with OCR0A compare.
        TIMSK = 1 << OCIE0A;
            
        // Enable interrupts
        sei();
    }
    
    // Note for timing needed in the micro and nano seconds, this will be slightly off due to the cycles needed
    // to add one to the timer1_millis in the ISR.
    timespan_t millis()
    {
        timespan_t r;
           
        // Must be atomic read so this call gets the correct value. This is needed b/c this is a multi-byte
        // number.
        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            r = timer1_millis;
        }
           
        return r;
    }
}}