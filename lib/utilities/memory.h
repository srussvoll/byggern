#if MCU=='atmega162'
#pragma once

namespace Utilities {
    /**
    * Sets the required bits for the external memory to work as intended.
    */
    void InitializeMemory();
}
#endif