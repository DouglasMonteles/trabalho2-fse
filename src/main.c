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

#define DEFAULT_TEMPERATURE 20

void handle_terminal_process(double temperature) {
  int user_command = 0;
  double external_temperature, internal_temperature, default_temperature;

  init_bme280();

  do {
    // get external temperature
    int result = get_temperature(&external_temperature);
    
    // Atualizando a referencia do PID
    pid_atualiza_referencia(temperature);

    if (result != BME280_RESPONSE_SUCCESS) {
      printf("Error ao tentar obter a temperatura externa com o bme280.\n");
      exit(1);
    }

    internal_temperature = (double) request_float_intern_temperature_message();

    if (internal_temperature < 0 || internal_temperature > 100) {
      internal_temperature = DEFAULT_TEMPERATURE;
    }

    default_temperature = internal_temperature;

    float cooler_power, resistor_power;
    float power = pid_controle(internal_temperature);

    if (power >= 0) {
      cooler_power = 0;
      resistor_power = power;
    } else {
      cooler_power = power * (-1);

      if (cooler_power < 40) {
        cooler_power = 40;
      }

      resistor_power = 0;
    }

    send_controller_sign(power);
    send_reference_sign((float) temperature);

    // LEDs

    // Csv

    printf("TI: %.2f TE: %.2f TR: %.2f\n", internal_temperature, external_temperature, temperature);

    handle_temperature_power(power);
    user_command = read_user_commands();

    usleep(400000); // 4 seg
  } while (user_command ==  0);

  handle_user_command(user_command);
}

int main(int argc, char **argv) {
  short option;
  signal(SIGINT, handle_interrupt);

  init_application_configs();

  do {
    option = handle_initial_menu();

    // option == 1: Terminal
    if (option == 1) {
      double temperature;

      printf("Informe o valor da temperatura que deseja que o forno alcance:\n");
      scanf("%lf", &temperature);

      send_system_state(1);       // liga / desliga o sistema
      send_working_status(1);     // funcionando / parado
      send_controller_mode(1);    // Controle via dashboard ou curva / terminal

      handle_terminal_process(temperature);
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