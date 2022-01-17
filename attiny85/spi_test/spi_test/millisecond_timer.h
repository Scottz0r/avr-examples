#pragma once

#include <inttypes.h>

namespace scottz0r { namespace ms {
    using timespan_t = uint32_t;
    
    void init();
    
    timespan_t millis();
}}
