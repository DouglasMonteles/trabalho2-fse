#ifndef _UART_MODBUS_H_
#define _UART_MODBUS_H_

#define FILE_UART "/dev/serial0"
#define CALC_CRC_SIZE 7
#define CALC_CRC_SEND_SIZE 7

#define INT 'd'
#define FLOAT 'f'
#define STR 's'

#define DEVICE_ADDRESS_CODE 0x01                              // device address
#define FUNCTION_REQUEST_CODE 0x23                            // function code
#define FUNCTION_SEND_CODE 0x16                               // function code
#define REQUEST_FLOAT_INTERN_TEMPERATURE_DATA_CODE 0xC1       // data type

#define REQUEST_INT_DATA_CODE 0xA1        // data type
#define REQUEST_FLOAT_DATA_CODE 0xA2      // data type
#define REQUEST_STR_DATA_CODE 0xA3        // data type
#define SEND_INT_DATA_CODE 0xB1           // data type
#define SEND_FLOAT_DATA_CODE 0xB2         // data type
#define SEND_STR_DATA_CODE 0xB3           // data type

int init_uart();
void config_uart(int uart0_filestream);

float request_float_intern_temperature_message(void);
void check_for_any_rx_bytes(int uart0_filestream, char type, void* p_out);

#endif