#ifndef _UART_MODBUS_H_
#define _UART_MODBUS_H_

#define FILE_UART "/dev/serial0"
#define CALC_CRC_SIZE 7
#define CALC_CRC_SEND_SIZE 11

#define INT 'd'
#define FLOAT 'f'
#define STR 's'

#define DEVICE_ADDRESS_CODE 0x01                              // device address
#define FUNCTION_REQUEST_CODE 0x23                            // function code
#define FUNCTION_SEND_CODE 0x16                               // function code

#define REQUEST_FLOAT_INTERN_TEMPERATURE_DATA_CODE 0xC1       // data type
#define REQUEST_FLOAT_REFERENCE_TEMPERATURE_DATA_CODE 0xC2    // data type
#define REQUEST_READ_USER_COMMANDS_DATA_CODE 0xC3             // data type

#define SEND_CONTROLLER_SIGN_DATA_CODE 0xD1                   // data type
#define SEND_REFERENCE_SIGN_DATA_CODE 0xD2                    // data type
#define SEND_SYSTEM_STATE_DATA_CODE 0xD3                      // data type
#define SEND_CONTROLLER_MODE_DATA_CODE 0xD4                   // data type

#define REQUEST_INT_DATA_CODE 0xA1        // data type
#define REQUEST_FLOAT_DATA_CODE 0xA2      // data type
#define REQUEST_STR_DATA_CODE 0xA3        // data type
#define SEND_INT_DATA_CODE 0xB1           // data type
#define SEND_FLOAT_DATA_CODE 0xB2         // data type
#define SEND_STR_DATA_CODE 0xB3           // data type

int init_uart();
void config_uart(int uart0_filestream);

float request_float_intern_temperature_message(void);
float request_float_reference_temperature_message(void);
int read_user_commands(void);
void send_controller_sign(int controller_sign);
void send_reference_sign(float reference_sign);
int send_system_state(char state);
int send_controller_mode(char mode);
void check_for_any_rx_bytes(int uart0_filestream, char type, void* p_out);

#endif