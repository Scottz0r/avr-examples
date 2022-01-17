#include "spi_interface.h"

#include <avr/io.h>
#include <inttypes.h>
#include "millisecond_timer.h"

namespace scottz0r { namespace spi {

    using namespace scottz0r::ms;

    enum class MessageState : uint8_t
    {
        Recv,
        Send    
    };

    // The amount of time to wait for clocks when sending a message.
    static constexpr timespan_t SEND_TIMEOUT_MS = 500;
    
    static MessageState s_curr_state = MessageState::Recv;
    static timespan_t s_send_start = 0;
    static uint8_t s_buff = 0;
    static bool s_has_buffer = false;
    static uint8_t s_last_recv_data = 0;

    // True if the USI Status Register Interrupt Flag is high.
    inline static bool spi_data_done()
    {
        return (USISR & (1 << USIOIF));
    }
    
    inline static void reset_usisr_clk()
    {
        // Write a 1 to the overflow interrupt flag to reset it to zero. Use an equals here to also
        // clear our the 4 bits in the counter to fully reset the clock count. Note that the non-counter
        // flags in this register require 1s to clear or are not used by 3-wire mode.
        USISR = (1 << USIOIF);
    }
    
    static SpiResult process_recv_state();
    static SpiResult process_send_state();
    static void reset_to_recv();
    
    void init()
    {
        // Set USI Control Register:
        // - USIWM0: three-write mode (Mode 01)
        // - USICS1: Extern clock source (default USICS0 and USICLK, so positive edge, SPI MODE 1).
        USICR = (1 << USIWM0) | (1 << USICS1);
                
        // Set Pin 6 (DO) as output (DDR = 1). Leave Pin 5 (DI) and Pin 7 (CLK) as input (DDR = 0)
        DDRB |= (1 << DDB1);
        
        // Clear contents of data register.
        USIDR = 0;
        
        // Reset the clock counter and interrupt flag.
        reset_usisr_clk();
        
        // Static module variables assumed to be initialized in declaration.
    }
    
    uint8_t get_recv_data()
    {
        return s_last_recv_data;
    }
    
    SpiResult process()
    {
        if (s_curr_state == MessageState::Recv)
        {
            return process_recv_state();
        }
        else if(s_curr_state == MessageState::Send)
        {
            return process_send_state();
        }
        
        // Unexpected state, so reset back to a receive state.
        reset_to_recv();
        return SpiResult::Error;
    }

    void send(uint8_t value)
    {
        // The the data register to the value to be sent.
        USIDR = value;
        
        // Change program into a sending state.
        s_curr_state = MessageState::Send;
        
        // Track when this message started the send for timeout purposes.
        s_send_start = millis();
        
        // Reset the USI interrupt flag and clock state so 8 bits can be sent.
        reset_usisr_clk();
    }
    
    void send16(uint16_t value)
    {
        // Save the MSB in the buffer
        s_buff = (uint8_t)(value >> 8);
        s_has_buffer = true;
        
        // Send the LSB first.
        uint8_t value_lsb = value & 0xFF;
        send(value_lsb );
    }
    
    static SpiResult process_recv_state()
    {
        if (spi_data_done())
        {
            // Save the result from the USI data register.
            // IMPORTANT: This must be read from the data register and not the buffer register. The buffer register
            // does not behave as expected and will introduce extra bits if used here.
            s_last_recv_data = USIDR;
            
            reset_to_recv();
            return SpiResult::DataAvailable;
        }

        return SpiResult::Ok;
    }
    
    static SpiResult process_send_state()
    {
        // Check for a timeout.
        timespan_t now = millis();
        
        timespan_t ellapsed = now - s_send_start;
        if (ellapsed >= SEND_TIMEOUT_MS)
        {
            reset_to_recv();
            return SpiResult::Timeout;
        }
        
        // If message not sent, do nothing else.
        if (!spi_data_done())
        {
            return SpiResult::Ok;
        }
        
        // Message sent. Check for buffered data.
        if (s_has_buffer)
        {
            s_has_buffer = false;
            send(s_buff);
            return SpiResult::Ok;
        }
        
        // No more data to send. Reset to RECV.
        reset_to_recv();
        
        return SpiResult::Ok;
    }
    
    static void reset_to_recv()
    {
        s_curr_state = MessageState::Recv;
        s_buff = 0;
        s_has_buffer = false;

        // Clear bits in the data register.
        USIDR = 0;

        reset_usisr_clk();
    }
}}
