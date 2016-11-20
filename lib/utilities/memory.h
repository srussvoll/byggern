#pragma once

#ifdef __AVR_ATmega162__

namespace Utilities {

    /**
    * Sets the required bits for the external memory to work as intended.
    */
    void InitializeMemory();
}
#endif