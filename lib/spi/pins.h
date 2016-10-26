#pragma once
/**
* Pins used to port the SPI from ATMega162 to ATMega2560
*/

#ifdef __AVR_ATmega162__

#define MOSI_DDR                            DDRB
#define MOSI_PIN                            DDB5
#define MISO_DDR                            DDRB
#define MISO_PIN                            DDB6
#define SCK_DDR                             DDRB
#define SCK_PIN                             DDB7
#define SS_DDR                              DDRB
#define SS_PIN                              DDB4

#else

#define MOSI_DDR                            DDRB
#define MOSI_PIN                            DDB2
#define MISO_DDR                            DDRB
#define MISO_PIN                            DDB3
#define SCK_DDR                             DDRB
#define SCK_PIN                             DDB1
#define SS_DDR                              DDRB
#define SS_PIN                              DDB0

#endif