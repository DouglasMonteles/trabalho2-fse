#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "routine_controller.h"
#include "pid.h"
#include "gpio.h"
#include "menu.h"
#include "uart_modbus.h"
#include "bme280_temperature.h"

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
  } while (user_command != 162);
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
}