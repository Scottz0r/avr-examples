#include "hwcfg.h"
#include "fast_lcd.h"

#include <avr/io.h>
#include <util/delay.h>

static void int_to_str(int value, char* buffer);
static void rpad(char* buffer, int size);

int main(void)
{
    lcd_init();    
    lcd_write_str("Hi AVR!");

    unsigned int counter = 0;
    char buffer[16];

    while (1) 
    {        
        _delay_ms(1000);

        int_to_str(counter, buffer);
        rpad(buffer, sizeof(buffer));

        lcd_set_cursor(1, 0);
        lcd_write_str(buffer);
        
        ++counter;
        
        if (counter == 20)
        {
            counter = 0;
        }
    }
}

static void rpad(char* buffer, int size)
{
    // Guard against bad size.
    if (size <= 0)
    {
        return;
    }
    
    int i = 0;
    
    for(; buffer[i] != 0; ++i);
    
    for(; i < size; ++i)
    {
        buffer[i] = ' ';
    }
    
    // Null terminate last position.
    buffer[size - 1] = 0;
}

// TODO - this probably is not very safe, but is kilobytes smaller than sprintf.
static void int_to_str(int value, char* buffer)
{
    int neg = 0;

    if (value < 0)
    {
        neg = 1;
        value = -value;
    }

    int v = value;
    int n = 0;

    do
    {
        int c = v % 10;
        buffer[n] = '0' + c;
        ++n;
        v /= 10;
    } while (v);

    if (neg)
    {
        buffer[n] = '-';
        ++n;
    }

    buffer[n] = 0;

    for (int f = 0, b = n - 1; f < b; ++f, --b)
    {
        char tmp = buffer[f];
        buffer[f] = buffer[b];
        buffer[b] = tmp;
    }
}
