#include <stdio.h>
#include <unistd.h>  // Used for UART
#include <fcntl.h>   // Used for UART
#include <termios.h> // Used for UART
#include <string.h>
#include <stdlib.h>

#include "uart_modbus.h"
#include "crc16.h"

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

  printf("Buffers de memória criados!\n");

  if (uart0_filestream != -1) {
    printf("Escrevendo caracteres na UART ...");
    int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
    
    if (count < 0) {
      printf("UART TX error\n");
    } else {
      printf("escrito.\n");
    }
  }

  sleep(1);

  check_for_any_rx_bytes(uart0_filestream, FLOAT, &intern_temperature);
  close(uart0_filestream);

  return intern_temperature;
}

void check_for_any_rx_bytes(int uart0_filestream, char type, void* p_out) {
  //----- CHECK FOR ANY RX BYTES -----
  if (uart0_filestream != -1) {
    // Read up to 255 characters from the port if they are there
    unsigned char rx_buffer[256];
    int rx_length = read(uart0_filestream, (void *)rx_buffer, 255); // Filestream, buffer to store in, number of bytes to read (max)
    
    if (rx_length < 0) {
      printf("Erro na leitura.\n"); // An error occured (will occur if there are no bytes)
    } else if (rx_length == 0) {
      printf("Nenhum dado disponível.\n"); // No data waiting
    } else {
      // Bytes received
      rx_buffer[rx_length] = '\0';

      short crc_buffer, crc_response;
      
      switch (type) {
        case INT: {
          int response;

          memcpy(&response, &rx_buffer[3], sizeof(response));
          printf("%i Bytes lidos : %d\n", rx_length, response);
          memcpy(&crc_buffer, &rx_buffer[7], sizeof(short));
          crc_response = calcula_CRC(rx_buffer, 7);
          break;
        }

        case FLOAT: {
          float response;

          memcpy(&response, &rx_buffer[3], sizeof(response));
          printf("%i Bytes lidos : %f\n", rx_length, response);
          memcpy(&crc_buffer, &rx_buffer[7], sizeof(short));
          crc_response = calcula_CRC(rx_buffer, 7);

          *(float*) p_out = response;
          break;
        }
        
        case STR: {
          char response[255];

          memcpy(&response, &rx_buffer[4], rx_buffer[3]);
          printf("%i Bytes lidos : %s\n", rx_length, response);
          memcpy(&crc_buffer, &rx_buffer[5+rx_buffer[3]], sizeof(short));
          crc_response = calcula_CRC(rx_buffer, 5+rx_buffer[3]);
          break;
        }

        default:
          printf("Erro na passagem do tipo de dado!\n");
      }

      if (crc_response == crc_buffer) {
        printf("CRC Validado: %i\n", crc_buffer);
      } else {
        printf("CRC INVALIDO. \nCRC buffer: %i != CRC CALC: %i\n", crc_buffer, crc_response);
      }
    }
  }

  printf("\n");
}

int init_uart() {
  int uart0_filestream = -1;
  uart0_filestream = open(FILE_UART, O_RDWR | O_NOCTTY | O_NDELAY); // Open in non blocking read/write mode
  
  if (uart0_filestream == -1) {
    printf("Erro - Não foi possível iniciar a UART.\n");
  } else {
    printf("UART inicializada!\n");
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