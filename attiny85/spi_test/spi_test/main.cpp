#include "prj_hardware.h"
#include "millisecond_timer.h"
#include "led_controller.h"
#include "spi_interface.h"
#include "tmp36_adc.h"

#include <inttypes.h>

using namespace scottz0r;

using scottz0r::led_ctrl::LedMode;
using scottz0r::spi::SpiResult;

static void handle_tmp36_read();

/*
    TODO:
    
    1) Test LED driver in isolation to make sure that's working. >> DONE
    
    2) Try to hook up SPI with the Mega to drive the LEDs.
    
    3) See if a TMP32 (or w/e) can be hooked up to send data out.
*/
int main(void)
{
    ms::init();
    spi::init();
    tmp36::init();
        
    led_ctrl::change_mode(LedMode::Off);
    
    for(;;)
    {
        led_ctrl::process();
        tmp36::process();
                
        SpiResult msg_state = spi::process();
        if (msg_state == SpiResult::DataAvailable)
        {
            uint8_t data = spi::get_recv_data();
            
            switch(data)
            {
            case 0x01:
                led_ctrl::change_mode(LedMode::On);
                spi::send(0x00);
                break;
            case 0x02:
                led_ctrl::change_mode(LedMode::Off);
                spi::send(0x00);
                break;
            case 0x03:
                spi::send16(0x1234);
                break;
            case 0x10:
                tmp36::start();
                spi::send(0x00);
                break;
            case 0x11:
                handle_tmp36_read();
                break;             
            default:
                led_ctrl::change_mode(LedMode::Pulse500);
                spi::send(0xFF);
                break;
            }
        }
    }
}

static void handle_tmp36_read()
{    
    int16_t temperature = 0;
    if (tmp36::last_measurement(temperature))
    {
        // Case to 16-bit integer for transmission. Assuming 2s complement, this is okay.
        spi::send16((uint16_t)temperature);
        
        // Clear out the value to not have stale reads.
        tmp36::clear();
    }
    else
    {
        // Send a "bad temp" value.
        spi::send16(0x8000);
    }
}
