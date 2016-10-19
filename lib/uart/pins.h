#pragma once

/**
* Pins used to port the UART from ATMega162 to ATMega2560
*/

#ifdef __AVR_ATmega162__

#define DATA_REGISTER                       UDR0
#define BAUD_RATE_HIGHER                    UBRR0H
#define BAUD_RATE_LOWER                     UBRR0L
#define USART_CONTROL_STATUS_REGISTER_0B    UCSR0B
#define USART_CONTROL_STATUS_REGISTER_0C    UCSR0C
#define INTERRUPT_ENABLE_0                  UDRIE0
#define RECEIVE_ENABLE_0                    RXEN0
#define TRANSMIT_ENABLE_0                   TXEN0
#define REGISTER_SELECT_0                   URSEL0
#define STOP_BIT_SELECT_0                   USBS0
#define CHARATCER_SIZE_0                    UCSZ00
#define MYUBRR                              FOSC/16/BAUD-1

#else

#define DATA_REGISTER                       UDR0
#define BAUD_RATE_HIGHER                    UBRR0H
#define BAUD_RATE_LOWER                     UBRR0L
#define USART_CONTROL_STATUS_REGISTER_0B    UCSR0B
#define USART_CONTROL_STATUS_REGISTER_0C    UCSR0C
#define INTERRUPT_ENABLE_0                  UDRIE0
#define RECEIVE_ENABLE_0                    RXEN0
#define TRANSMIT_ENABLE_0                   TXEN0
#define REGISTER_SELECT_0                   -1
#define STOP_BIT_SELECT_0                   USBS0
#define CHARATCER_SIZE_0                    UCSZ00

#endif