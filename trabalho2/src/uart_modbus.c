#include <stdio.h>
#include <unistd.h>  // Used for UART
#include <fcntl.h>   // Used for UART
#include <termios.h> // Used for UART
#include <string.h>
#include <stdlib.h>

#include "uart_modbus.h"
#include "crc16.h"
#include "debug.h"

float request_float_intern_temperature_message(void) {
  float intern_temperature = -100.f;
  int uart0_filestream = init_uart();

  unsigned char tx_buffer[20];
  unsigned char *p_tx_buffer;

  // Request intern temperature
  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = DEVICE_ADDRESS_CODE;
  *p_tx_buffer++ = FUNCTION_REQUEST_CODE;
  *p_tx_buffer++ = REQUEST_FLOAT_INTERN_TEMPERATURE_DATA_CODE;

  // Last four digits of the registration number
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 0;
  *p_tx_buffer++ = 0;

  short crc_value = calcula_CRC(tx_buffer, CALC_CRC_SIZE);
  memcpy(p_tx_buffer, &crc_value, sizeof(crc_value));
  
  p_tx_buffer += sizeof(crc_value);

  log_info("Buffers de memória criados!");

  if (uart0_filestream != -1) {
    log_info("Escrevendo caracteres na UART ...");
    int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
    
    if (count < 0) {
      log_error("UART TX error");
    } else {
      log_info("escrito.");
    }
  }

  sleep(1);

  check_for_any_rx_bytes(uart0_filestream, FLOAT, &intern_temperature);
  close(uart0_filestream);

  return intern_temperature;
}

float request_float_reference_temperature_message(void) {
  float reference_temperature = -100.f;
  int uart0_filestream = init_uart();

  unsigned char tx_buffer[20];
  unsigned char *p_tx_buffer;

  // Request reference temperature
  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = DEVICE_ADDRESS_CODE;
  *p_tx_buffer++ = FUNCTION_REQUEST_CODE;
  *p_tx_buffer++ = REQUEST_FLOAT_REFERENCE_TEMPERATURE_DATA_CODE;

  // Last four digits of the registration number
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 0;
  *p_tx_buffer++ = 0;

  short crc_value = calcula_CRC(tx_buffer, CALC_CRC_SIZE);
  memcpy(p_tx_buffer, &crc_value, sizeof(crc_value));
  
  p_tx_buffer += sizeof(crc_value);

  log_info("Buffers de memória criados!");

  if (uart0_filestream != -1) {
    log_info("Escrevendo caracteres na UART ...");
    int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
    
    if (count < 0) {
      log_error("UART TX error");
    } else {
      log_info("escrito.");
    }
  }

  sleep(1);

  check_for_any_rx_bytes(uart0_filestream, FLOAT, &reference_temperature);
  close(uart0_filestream);

  return reference_temperature;
}

int read_user_commands(void) {
  int command = -1;
  int uart0_filestream = init_uart();

  unsigned char tx_buffer[20];
  unsigned char *p_tx_buffer;

  // Request reference temperature
  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = DEVICE_ADDRESS_CODE;
  *p_tx_buffer++ = FUNCTION_REQUEST_CODE;
  *p_tx_buffer++ = REQUEST_READ_USER_COMMANDS_DATA_CODE;

  // Last four digits of the registration number
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 0;
  *p_tx_buffer++ = 0;

  short crc_value = calcula_CRC(tx_buffer, CALC_CRC_SIZE);
  memcpy(p_tx_buffer, &crc_value, sizeof(crc_value));
  
  p_tx_buffer += sizeof(crc_value);

  log_info("Buffers de memória criados!");

  if (uart0_filestream != -1) {
    log_info("Escrevendo caracteres na UART ...");
    int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
    
    if (count < 0) {
      log_error("UART TX error");
    } else {
      log_info("escrito.");
    }
  }

  sleep(1);

  check_for_any_rx_bytes(uart0_filestream, INT, &command);
  close(uart0_filestream);

  return command;
}

void send_controller_sign(int controller_sign) {
  int uart0_filestream = init_uart();

  unsigned char tx_buffer[20];
  unsigned char *p_tx_buffer;

  // Send controller sign (int)
  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = DEVICE_ADDRESS_CODE;
  *p_tx_buffer++ = FUNCTION_SEND_CODE;
  *p_tx_buffer++ = SEND_CONTROLLER_SIGN_DATA_CODE;

  // Last four digits of the registration number
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 0;
  *p_tx_buffer++ = 0;

  memcpy(p_tx_buffer, &controller_sign, sizeof(controller_sign));
  p_tx_buffer+=sizeof(controller_sign);

  short crc_value = calcula_CRC(tx_buffer, CALC_CRC_SEND_SIZE);
  memcpy(p_tx_buffer, &crc_value, sizeof(crc_value));
  
  p_tx_buffer += sizeof(crc_value);

  log_info("Buffers de memória criados!");

  if (uart0_filestream != -1) {
    log_info("Escrevendo caracteres na UART ...");
    int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
    
    if (count < 0) {
      log_error("UART TX error");
    } else {
      log_info("escrito");
    }
  }

  sleep(1);
  close(uart0_filestream);
}

void send_reference_sign(float reference_sign) {
  int uart0_filestream = init_uart();

  unsigned char tx_buffer[20];
  unsigned char *p_tx_buffer;

  // Send a float
  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = DEVICE_ADDRESS_CODE;
  *p_tx_buffer++ = FUNCTION_SEND_CODE;
  *p_tx_buffer++ = SEND_REFERENCE_SIGN_DATA_CODE;

  // Last four digits of the registration number
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 0;
  *p_tx_buffer++ = 0;

  memcpy(p_tx_buffer, &reference_sign, sizeof(reference_sign));
  p_tx_buffer+=sizeof(reference_sign);
  
  short crc_value = calcula_CRC(tx_buffer, CALC_CRC_SEND_SIZE);
  memcpy(p_tx_buffer, &crc_value, sizeof(crc_value));
  
  p_tx_buffer += sizeof(crc_value);

  log_info("Buffers de memória criados!");

  if (uart0_filestream != -1) {
    log_info("Escrevendo caracteres na UART ...");
    int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
    
    if (count < 0) {
      log_error("UART TX error");
    } else {
      log_info("escrito.");
    }
  }

  sleep(1);
  close(uart0_filestream);
}

int send_system_state(char state) {
  int system_state = -1;
  int uart0_filestream = init_uart();

  unsigned char tx_buffer[20];
  unsigned char *p_tx_buffer;

  // Send controller sign (int)
  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = DEVICE_ADDRESS_CODE;
  *p_tx_buffer++ = FUNCTION_SEND_CODE;
  *p_tx_buffer++ = SEND_SYSTEM_STATE_DATA_CODE;

  // Last four digits of the registration number
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 0;
  *p_tx_buffer++ = 0;

  memcpy(p_tx_buffer, &state, sizeof(state));
  p_tx_buffer+=sizeof(state);

  short crc_value = calcula_CRC(tx_buffer, CALC_CRC_SEND_SIZE - 3);
  memcpy(p_tx_buffer, &crc_value, sizeof(crc_value));
  
  p_tx_buffer += sizeof(crc_value);

  log_info("Buffers de memória criados!");

  if (uart0_filestream != -1) {
    log_info("Escrevendo caracteres na UART ...");
    int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
    
    if (count < 0) {
      log_error("UART TX error");
    } else {
      log_info("escrito.");
    }
  }

  sleep(1);

  check_for_any_rx_bytes(uart0_filestream, INT, &system_state);
  close(uart0_filestream);

  return system_state;
}

int send_controller_mode(char mode) {
  int controller_mode = -1;
  int uart0_filestream = init_uart();

  unsigned char tx_buffer[20];
  unsigned char *p_tx_buffer;

  // Send controller sign (int)
  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = DEVICE_ADDRESS_CODE;
  *p_tx_buffer++ = FUNCTION_SEND_CODE;
  *p_tx_buffer++ = SEND_CONTROLLER_MODE_DATA_CODE;

  // Last four digits of the registration number
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 0;
  *p_tx_buffer++ = 0;

  memcpy(p_tx_buffer, &mode, sizeof(mode));
  p_tx_buffer+=sizeof(mode);

  short crc_value = calcula_CRC(tx_buffer, CALC_CRC_SEND_SIZE - 3);
  memcpy(p_tx_buffer, &crc_value, sizeof(crc_value));
  
  p_tx_buffer += sizeof(crc_value);

  log_info("Buffers de memória criados!");

  if (uart0_filestream != -1) {
    log_info("Escrevendo caracteres na UART ...");
    int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
    
    if (count < 0) {
      log_error("UART TX error");
    } else {
      log_info("escrito.");
    }
  }

  sleep(1);

  check_for_any_rx_bytes(uart0_filestream, INT, &controller_mode);
  close(uart0_filestream);

  return controller_mode;
}

int send_working_status(char status) {
  int working_status = -1;
  int uart0_filestream = init_uart();

  unsigned char tx_buffer[20];
  unsigned char *p_tx_buffer;

  // Send controller sign (int)
  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = DEVICE_ADDRESS_CODE;
  *p_tx_buffer++ = FUNCTION_SEND_CODE;
  *p_tx_buffer++ = SEND_WORKING_STATUS_DATA_CODE;

  // Last four digits of the registration number
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 0;
  *p_tx_buffer++ = 0;

  memcpy(p_tx_buffer, &status, sizeof(status));
  p_tx_buffer+=sizeof(status);

  short crc_value = calcula_CRC(tx_buffer, CALC_CRC_SEND_SIZE - 3);
  memcpy(p_tx_buffer, &crc_value, sizeof(crc_value));
  
  p_tx_buffer += sizeof(crc_value);

  log_info("Buffers de memória criados!");

  if (uart0_filestream != -1) {
    log_info("Escrevendo caracteres na UART ...");
    int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
    
    if (count < 0) {
      log_error("UART TX error");
    } else {
      log_info("escrito.");
    }
  }

  sleep(1);

  check_for_any_rx_bytes(uart0_filestream, INT, &working_status);
  close(uart0_filestream);

  return working_status;
}

float send_room_temperature(float room_temperature) {
  float temperature = 0.f;
  int uart0_filestream = init_uart();

  unsigned char tx_buffer[20];
  unsigned char *p_tx_buffer;

  // Send controller sign (int)
  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = DEVICE_ADDRESS_CODE;
  *p_tx_buffer++ = FUNCTION_SEND_CODE;
  *p_tx_buffer++ = SEND_ROOM_TEMPERATURE_DATA_CODE;

  // Last four digits of the registration number
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 2;
  *p_tx_buffer++ = 0;
  *p_tx_buffer++ = 0;

  memcpy(p_tx_buffer, &room_temperature, sizeof(room_temperature));
  p_tx_buffer+=sizeof(room_temperature);

  short crc_value = calcula_CRC(tx_buffer, CALC_CRC_SEND_SIZE);
  memcpy(p_tx_buffer, &crc_value, sizeof(crc_value));
  
  p_tx_buffer += sizeof(crc_value);

  log_info("Buffers de memória criados!");

  if (uart0_filestream != -1) {
    log_info("Escrevendo caracteres na UART ...");
    int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
    
    if (count < 0) {
      log_error("UART TX error");
    } else {
      log_info("escrito.");
    }
  }

  sleep(1);

  check_for_any_rx_bytes(uart0_filestream, FLOAT, &temperature);
  close(uart0_filestream);

  return temperature;
}

void check_for_any_rx_bytes(int uart0_filestream, char type, void* p_out) {
  //----- CHECK FOR ANY RX BYTES -----
  if (uart0_filestream != -1) {
    // Read up to 255 characters from the port if they are there
    unsigned char rx_buffer[256];
    int rx_length = read(uart0_filestream, (void *)rx_buffer, 255); // Filestream, buffer to store in, number of bytes to read (max)
    
    if (rx_length < 0) {
      log_error("Erro na leitura."); // An error occured (will occur if there are no bytes)
    } else if (rx_length == 0) {
      log_warn("Nenhum dado disponível."); // No data waiting
    } else {
      // Bytes received
      rx_buffer[rx_length] = '\0';

      short crc_buffer, crc_response;
      
      switch (type) {
        case INT: {
          int response;

          memcpy(&response, &rx_buffer[3], sizeof(response));
          #ifndef IS_DEBUG_ENABLED
            printf("%i Bytes lidos : %d\n", rx_length, response);
          #endif
          memcpy(&crc_buffer, &rx_buffer[7], sizeof(short));
          crc_response = calcula_CRC(rx_buffer, 7);

          *(int*) p_out = response;
          break;
        }

        case FLOAT: {
          float response;

          memcpy(&response, &rx_buffer[3], sizeof(response));
          #ifndef IS_DEBUG_ENABLED
            printf("%i Bytes lidos : %f\n", rx_length, response);
          #endif
          memcpy(&crc_buffer, &rx_buffer[7], sizeof(short));
          crc_response = calcula_CRC(rx_buffer, 7);

          *(float*) p_out = response;
          break;
        }
        
        case STR: {
          char response[255];

          memcpy(&response, &rx_buffer[4], rx_buffer[3]);
          #ifndef IS_DEBUG_ENABLED
            printf("%i Bytes lidos : %s\n", rx_length, response);
          #endif
          memcpy(&crc_buffer, &rx_buffer[5+rx_buffer[3]], sizeof(short));
          crc_response = calcula_CRC(rx_buffer, 5+rx_buffer[3]);
          break;
        }

        default:
          log_error("Erro na passagem do tipo de dado!");
      }

      if (crc_response == crc_buffer) {
        #ifndef IS_DEBUG_ENABLED
          printf("CRC Validado: %i\n", crc_buffer);
        #endif

      } else {
        #ifndef IS_DEBUG_ENABLED
          printf("CRC INVALIDO. \nCRC buffer: %i != CRC CALC: %i\n", crc_buffer, crc_response);
        #endif
      }
    }
  }

  printf("\n");
}

int init_uart() {
  int uart0_filestream = -1;
  uart0_filestream = open(FILE_UART, O_RDWR | O_NOCTTY | O_NDELAY); // Open in non blocking read/write mode
  
  if (uart0_filestream == -1) {
    log_error("Erro - Não foi possível iniciar a UART.");
  } else {
    log_info("UART inicializada!");
  }

  config_uart(uart0_filestream);

  return uart0_filestream;
}

void config_uart(int uart0_filestream) {
  struct termios options;
  
  tcgetattr(uart0_filestream, &options);
  options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; // Set baud rate
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(uart0_filestream, TCIFLUSH);
  tcsetattr(uart0_filestream, TCSANOW, &options);
}