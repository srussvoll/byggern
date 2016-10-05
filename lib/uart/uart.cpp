
#include "lib/uart/uart.h"

void USART0_UDRE_vect(){
	UART& uart = UART::GetInstance();
	uint8_t byte;

	if (uart.ReadByteFromOutputStream(byte)) {
		UDR0 = byte;
	} else {
        // Turn off interrupts.
        UCSR0B &= ~(1 << UDRIE0);
        uart.ongoing_transmission = false;
    }
}

void UART::Init(uint16_t baud_rate) {
    baud_rate = (uint16_t) (FOSC / 16 / baud_rate - 1);
	UBRR0H = (uint8_t)(baud_rate >> 8);
	UBRR0L = (uint8_t)(baud_rate);

	// Sets the transmitter and receiver register
	// If only receive set only TXEN0
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);


	// Sets format to eight data bits and two stop bits
	UCSR0C = (1<<URSEL0)|(3<<UCSZ00)|(1<<USBS0);

}

UART::UART(): Stream(1,128) {
	sei();
}

void UART::Write(uint8_t *string, uint16_t size) {
    Stream::Write(string, size);

    // Initialize transmission
    this->initialize_transmission();
}

void UART::initialize_transmission() {
    if (!this->ongoing_transmission) {
        this->ongoing_transmission = true;

        // Read byte
        uint8_t byte;
        Stream::ReadByteFromOutputStream(byte);

        // Write data
        UDR0 = byte;

        // Enable interrupts
        UCSR0B |= (1 << UDRIE0);
    }
}
