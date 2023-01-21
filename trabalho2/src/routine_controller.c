#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "routine_controller.h"
#include "pid.h"
#include "gpio.h"
#include "menu.h"
#include "uart_modbus.h"
#include "bme280_temperature.h"
#include "csv_controller.h"

void handle_terminal_process(double temperature) {
  int user_command = 0;

  // Atualizando a referencia do PID
  pid_atualiza_referencia(temperature);
  send_reference_sign(temperature);

  send_system_state(1);
  send_working_status(1);

  do {
    handle_potentiometer_process();

    sleep(1); // 1 seg
    user_command = read_user_commands();
    handle_user_command(user_command);
  } while (user_command != 162 && user_command != 164); // desligar ou parar o forno
}

void handle_potentiometer_process() {
  double temperature;
  int result = get_temperature(&temperature);

  if (result != BME280_RESPONSE_SUCCESS) {
    printf("Erro na leitura do bme280\n");
    exit(1);
  }

  pid_configura_constantes(Kp_DEFAULT, Ki_DEFAULT, Kd_DEFAULT); // default values

  float reference_temperature = get_reference_temperature();
  printf("Temperatura de referencia: %lf\n", reference_temperature);
  pid_atualiza_referencia(reference_temperature);

  float internal_temperature = request_float_intern_temperature_message();

  if (internal_temperature < 0 || internal_temperature > 100) {
    internal_temperature = 25;
  }

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

  printf("POWER DO POTENCIOMETRO: %f\n", power);
  send_controller_sign((int) power);
  handle_temperature_power(power);

  printf("TI: %.2f TR: %.2f\n", internal_temperature, reference_temperature);
  
  logger_system_data data;

  data.intern_temperature = internal_temperature;
  data.external_temperature = temperature;
  data.user_defined_temperature = reference_temperature;
  data.resistor_active_percent = resistor_power;
  data.cooler_active_percent = cooler_power;

  register_system_logs_in_csv(data);
}

void handle_temperature_curve_process() {
  int user_command = 0;

  int time_in_seconds[800];
  float temperature[800];
  int counter_time = 0, counter_temperature = 0, current_second = 0;

  int file_data_size = obtain_temperature_curve_from_csv(time_in_seconds, temperature);

  if (file_data_size <= 0) {
    printf("Nao ha dados no csv fornecido para a leitura do tempo e temperatura.\n");
    return;
  }

  int current_time_in_seconds = time_in_seconds[0];
  float current_temperature = temperature[0];

  send_system_state(1);
  send_working_status(1);

  do {
    // Atualizando a referencia do PID
    pid_atualiza_referencia(current_temperature);
    send_reference_sign(current_temperature);

    handle_potentiometer_process();

    if (file_data_size == 11) {
      printf("Ultima linha lida: %d %f\n", current_time_in_seconds, current_temperature);
    }
    
    usleep(500000); // 0.5 seconds
    current_second++;

    printf("\n\n\nTempo: %d, Temperatura: %f\n\n\n", current_second, current_temperature);

    if (current_second == time_in_seconds[counter_time+1]) {
      current_time_in_seconds = time_in_seconds[++counter_time];
      current_temperature = temperature[++counter_temperature];
    }

    if (current_second == 600) {
      break;
    }
  } while (counter_temperature != (file_data_size - 1) && counter_time != (file_data_size - 1)); // desligar ou parar o forno
}
