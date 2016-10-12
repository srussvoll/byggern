#include "lib/uart/uart.h"
#include "lib/uart/pins.h"

void USART0_UDRE_vect(){
	UART& uart = UART::GetInstance();
	uint8_t byte;

	if (uart.ReadByteFromOutputStream(byte)) {
		DATA_REGISTER = byte;
	} else {
        // Turn off interrupts.
        USART_CONTROL_STATUS_REGISTER_0B &= ~(1 << INTERRUPT_ENABLE_0);
        uart.ongoing_transmission = false;
    }
}

void UART::Init(uint16_t baud_rate) {
    baud_rate = (uint16_t) (FOSC / 16 / baud_rate - 1);
    BAUD_RATE_HIGHER = (uint8_t)(baud_rate >> 8);
    BAUD_RATE_LOWER = (uint8_t)(baud_rate);

	// Sets the transmitter and receiver register
	// If only receive set only TXEN0
    USART_CONTROL_STATUS_REGISTER_0B = (1<<RECEIVE_ENABLE_0)|(1<<TRANSMIT_ENABLE_0);


	// Sets format to eight data bits and two stop bits
#if MCU == ATmega162
    USART_CONTROL_STATUS_REGISTER_0C = (1<<REGISTER_SELECT_0)|(3<<CHARATCER_SIZE_0)|(1<<STOP_BIT_SELECT_0);
#elif MCU == ATmega2560
    USART_CONTROL_STATUS_REGISTER_0C = (3<<CHARATCER_SIZE_0)|(1<<STOP_BIT_SELECT_0);
#endif

}

UART::UART(): Stream(1,128) {
	sei();
}

void UART::Write(uint8_t *string, uint16_t size) {
    Stream::Write(string, size);

    // Start the transmission
    this->StartTransmission();


}

inline void UART::StartTransmission() {
    if (!this->ongoing_transmission) {
        this->ongoing_transmission = true;

        // Read byte
        uint8_t byte;
        Stream::ReadByteFromOutputStream(byte);

        // Write data
        DATA_REGISTER = byte;

        // Enable interrupts
        USART_CONTROL_STATUS_REGISTER_0B |= (1 << INTERRUPT_ENABLE_0);
    }
}
