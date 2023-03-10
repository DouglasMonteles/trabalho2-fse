#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "main.h"
#include "gpio.h"
#include "menu.h"
#include "pid.h"
#include "uart_modbus.h"
#include "bme280_temperature.h"
#include "routine_controller.h"
#include "debug.h"

int main(int argc, char **argv) {
  short option;
  signal(SIGINT, handle_interrupt);

  init_application_configs();

  do {
    option = handle_initial_menu();

    // option == 1: Terminal
    if (option == 1) {
      send_controller_mode(1);      // Controle via curva / terminal
      
      short terminal_option = 1;

      printf("\n");
      printf("[1] - Informar manualmente uma temperatura de referencia.\n");
      printf("[2] - Utilizar o arquivo de curva (csv) para definir a temperatura (graus Celsius) em um dado tempo (segundos)\n");
      printf("\n");

      printf("Informe o numero da operacao desejada ou Ctrl+c para finalizar:\n");
      scanf(" %hd", &terminal_option);

      if (terminal_option == 1) {
        handle_terminal_process();
      } else if (terminal_option == 2) {
        printf("O arquivo de curva de temperatura sera utilizado.\n");
        handle_temperature_curve_process();
      } else {
        printf("Opcao invalida!\n");
      }
    } else if (option == 2) { // Dashboard
      handle_dashboard_process();
    }
  } while (option == 1 || option == 2);

  return 0;
}

void init_application_configs() {
  // GPIO pin outputs
  config_gpio_outputs();

  // Config Kp, Ki and Kd constants
  config_pid();

  // BME 280
  init_bme280();

  // Reset dashboard values
  send_system_state(0);       // liga / desliga o sistema
  send_working_status(0);     // liga / desliga
  send_controller_mode(0);    // Controle via dashboard ou curva / terminal
}

void handle_interrupt(int signal) {
  printf("\n\nInterrompendo o programa e finalizando os processos abertos...\n");
  
  handle_gpio_interrupt(); // GPIO
  bme280_driver_close();   // BME280 driver

  send_system_state(OFF);       // liga / desliga o sistema
  printf("Forno desligado!\n");
  send_working_status(OFF);     // liga / desliga
  printf("Funcionamento do forno desligado!\n");
  send_controller_mode(OFF);    // Controle via dashboard ou curva / terminal
  printf("Modo de controle via dashboard!\n");

  printf("\n\nTodos os processos utilizados pelo programa foram finalizados com sucesso!\n\n");
  exit(0);
}