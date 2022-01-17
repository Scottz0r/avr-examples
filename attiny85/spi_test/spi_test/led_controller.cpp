#include "led_controller.h"
#include "millisecond_timer.h"

#include "prj_hardware.h"

namespace scottz0r { namespace led_ctrl {
    using namespace scottz0r::ms;
    
    static constexpr timespan_t pulse_span = 500;
    
    LedMode s_led_mode = LedMode::Off;
    timespan_t s_led_chg = 0;
    
    void init()
    {
        // Set pin as output (DDR = 1)
        LED_DDR |= (1 << LED_PIN);
    }
    
    void process()
    {        
        if (s_led_mode == LedMode::Pulse500)
        {
            timespan_t now = millis();
            timespan_t ellapsed = now - s_led_chg;
            
            if (ellapsed >= pulse_span)
            {
                LED_PORT ^= (1 << LED_PIN);
                s_led_chg = now;
            }
        }
    }
    
    void change_mode(LedMode new_mode)
    {
        s_led_mode = new_mode;
        
        switch (s_led_mode)
        {
        case LedMode::On:
            LED_PORT |= (1 << LED_PIN);
            break;
        case LedMode::Off:
            LED_PORT &= ~(1 << LED_PIN);
            break;
        default:
            // Nothing to handle
            break;
        }
    }
}}
