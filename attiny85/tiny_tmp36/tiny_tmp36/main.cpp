#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>

// Ready indication pin, Pin 3
#define READY_PIN PB4

// Ready state, pull pin low.
#define READY_HIGH() (PORTB |= (1 << READY_PIN))

// Not eady state, pull pin high.
#define READY_LOW() (PORTB &= ~(1 << READY_PIN))

static void loop();
static void init_usir();
static void init_adc();
static int16_t adc_to_temperature(uint16_t adc);

int main(void)
{
    // Initialize output ready pin as an output (1). Initialize as 1, not ready.
    DDRB |= (1 << READY_PIN);
    READY_LOW();
    
    init_usir();
    init_adc();
    
    for(;;)
    {
        loop();
    }
}

static void loop()
{    
    // Set ADC start conversion bit.
    ADCSRA |= (1 << ADSC);
    
    // Read from ADC. Blocking.
    while (ADCSRA & (1 << ADSC));
    
    // Save ADC result.
    int16_t data = adc_to_temperature(ADC);
        
    // Put lsb into USI output register.
    USIDR = (char)(data & 0xFF);
    USISR = (1 << USIOIF);
    
    // Indicate ready to read.
    READY_HIGH();
        
    // Block until 8 bits read from USI.
    while (!(USISR & (1 << USIOIF)));
    
    READY_LOW();
    
    // Put msb into USI output register.
    USIDR = (char)(data >> 8);
    USISR = (1 << USIOIF);
        
    // Indicate ready to read.
    READY_HIGH();
    
    // Block until 8 bits read from USI.
    while (!(USISR & (1 << USIOIF)));
    
    READY_LOW();
}

static void init_usir()
{
    // Set USI Control Register:
    // - USIWM0: three-write mode (Mode 01)
    // - USICS1: Extern clock source (default USICS0 and USICLK, so positive edge, SPI MODE 1).
    USICR = (1 << USIWM0) | (1 << USICS1);
        
    // Pin 6 = DO output (set in main initialize). Pin 5 (DI) and Pin 7 (CLK) as input (DDR = 0).
    DDRB |= (1 << PB1);
    
    // Clear contents of data register.
    USIDR = 0;
}

static void init_adc()
{
    // Set Mux to ADC3/Pin2. Keep all other settings.
    ADMUX = (1 << MUX1) | (1 << MUX0);
                
    // Enable ADC and Set prescalar to 8.
    ADCSRA |= (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);
}

static int16_t adc_to_temperature(uint16_t adc)
{
    float voltage = adc * (5000.0 / 1024.0);
    float tempr = (voltage - 500) / 10.0;
    
    int16_t temp_100ths = (int16_t)(tempr * 100.0);
    return temp_100ths;
}
