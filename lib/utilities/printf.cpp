#include "printf.h"
#include <avr/interrupt.h>

namespace{
    Stream *output_stream;
    int put(char c, FILE* f) {
        // Enforce printf when debugging!
        #if DEBUG
        sei();
        #endif

        output_stream->Write((uint8_t *) &c, 1);
        while (output_stream->GetOutputBufferLength() != 0) {}
        return 0;
    }
}
namespace Utilities{
    void enable_printf(Stream &stream){
        output_stream = &stream;
        fdevopen(&put, NULL);
    }
}
