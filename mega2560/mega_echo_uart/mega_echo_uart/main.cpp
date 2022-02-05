#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>

// This value from the data sheet. Can also be found with ((F_CPU / (16 * BAUD)) - 1)
#define UBRR_9600 (uint16_t)103

static void init_uart();

int main(void)
{
    init_uart();
    
    // Send a "Hi" message
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = 'H';
    
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = 'I';
    
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = '\r';
    
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = '\n';
    
    for (;;)
    {
        // Block until a character received
        while (!(UCSR0A & (1 << RXC0)));
        
        // Read receive buffer.
        char data = UDR0;
        
        // Block until data register empty.
        while (!(UCSR0A & (1 << UDRE0)));
        
        // Send put the data back into the UDR to be transmitted.
        UDR0 = data;
    }
}

// Initialize with settings that the Arduino Serial Monitor expects:
// - 9600 baud
// - 8 bits
// - 1 stop bit
// - no parity
//
// This will configure UART 0 (the one connected to the USB on Arduino boards).
static void init_uart()
{
    // Reset 0 register.
    // UCSR0A = 0;
    
    // Enable RX and TX
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    
    // Set 8 bits in character. Leave other bits zero for no parity and 1 stop bit.
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    
    // Baud rate. From Table 22-9 in datasheet.
    UBRR0 = UBRR_9600;
}
