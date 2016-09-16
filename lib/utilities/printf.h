#pragma once
#include "../stream/stream.h"
#include <stdio.h>
namespace Utilities{
    /**
     * Connects printf to the given stream
     * @param stream The stream to be connected to printf
     */
    void enable_printf(Stream &stream);
}
