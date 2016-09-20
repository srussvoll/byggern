#pragma once
#include "../stream/stream.h"
#include <stdio.h>
namespace Utilities{
    /**
     * Connects stdout to the specified stream, enabling printf().
     * @param stream The stream stdio (printf) should use.
     */
    void enable_printf(Stream &stream);
}
