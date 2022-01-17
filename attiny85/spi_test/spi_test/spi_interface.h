#pragma once

#include <inttypes.h>

namespace scottz0r { namespace spi {

    enum class SpiResult : uint8_t
    {
        Ok,
        DataAvailable,
        Timeout,
        Error
    };

    void init();

    uint8_t get_recv_data();
    
    SpiResult process();
    
    void send(uint8_t value);
    
    void send16(uint16_t value);

}}
