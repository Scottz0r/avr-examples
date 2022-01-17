#pragma once

#include <inttypes.h>

namespace scottz0r { namespace led_ctrl {

    enum class LedMode : uint8_t
    {
        Off,
        On,
        Pulse500
    };

    void init();
    
    void process();
    
    void change_mode(LedMode new_mode);
}}
