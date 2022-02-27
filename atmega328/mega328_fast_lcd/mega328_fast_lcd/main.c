#include "hwcfg.h"
#include "fast_lcd.h"
#include "fstr.h"

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    lcd_init();    
    lcd_write_str("Hi AVR! With Lib");

    unsigned int counter = 0;
    char buffer[16];
    fstr counter_str;
    fstr_init(&counter_str, buffer, sizeof(buffer));

    while (1) 
    {        
        _delay_ms(1000);

        fstr_itofstr(counter, &counter_str, 10);
        fstr_rpad(&counter_str);

        lcd_set_cursor(1, 0);
        lcd_write_str(counter_str.str);
        
        ++counter;
        
        if (counter == 20)
        {
            counter = 0;
        }
    }
}
