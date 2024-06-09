#ifndef UART_H
#define UART_H

#include "main.h"

extern uint8_t data_buffer[100]; // data buffer
extern uint8_t recvd_data; // receive buffer
extern uint32_t count; // count how many bytes are received


//count=0;

// UART-related function prototypes
void process_uart_command(uint8_t* cmd);

#endif /* UART_H */
