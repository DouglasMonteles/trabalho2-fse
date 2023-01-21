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

      printf("[1] - Informar manualmente uma temperatura de referencia.\n");
      printf("[2] - Utilizar o arquivo de curva (csv) para definir a temperatura (graus Celsius) em um dado tempo (segundos)\n");

      scanf(" %hd", &terminal_option);

      if (terminal_option == 1) {
        double temperature;

        printf("Informe o valor da temperatura que deseja que o forno alcance:\n");
        scanf("%lf", &temperature);

        handle_terminal_process(temperature);
      } else if (terminal_option == 2) {
        printf("O arquivo de curva de temperatura sera utilizado.\n");
        handle_temperature_curve_process();
      } else {
        printf("Opcao invalida!\n");
      }
    } else if (option == 2) { // Dashboard
    }
  } while (option == 1 || option == 2);

  // REMOVER
  while ((option = menu()) != EXIT) {
    handle_menu_option(option);
  }

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

  send_system_state(0);       // liga / desliga o sistema
  send_working_status(0);     // liga / desliga
  send_controller_mode(0);    // Controle via dashboard ou curva / terminal

  printf("Todos os processos utilizados pelo programa foram finalizados com sucesso\n\n");
  exit(0);
}