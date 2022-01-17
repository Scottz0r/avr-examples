#pragma once

#include <inttypes.h>

namespace scottz0r { namespace tmp36 {
    void init();
    
    void process();
    
    void start();
    
    void clear();
    
    bool last_measurement(int16_t& value);
    
    // TODO - remove
    int16_t brute_force();
}}
