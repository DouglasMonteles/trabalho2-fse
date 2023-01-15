#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <bcm2835.h>

#include "gpio.h"

short pin_status(int pin) {
  return bcm2835_gpio_lev(pin);
}

void turn_on_or_off_output(int output) {
  printf("O(A) %s estava: %s\n", get_gpio_name(output), pin_status(output) == LOW ? "DESLIGADO(A)" : "LIGADO(a)");

  if (pin_status(output) == HIGH) {
    bcm2835_gpio_write(output, LOW);
  } else {
    bcm2835_gpio_write(output, HIGH);
  }
  
  printf("O(A) %s agora esta: %s\n", get_gpio_name(output), pin_status(output) == LOW ? "DESLIGADO(A)" : "LIGADO(A)");
}

char* get_gpio_name(int pin) {
  switch (pin) {
    case RESISTOR_GPIO:
      return "RESISTOR";
    
    case COOLER_GPIO:
      return "VENTOINHA";

    default:
      return "UNKNOWN";
  }
}

void handle_gpio_interrupt() {
  // shutdown the resistor and fan
  bcm2835_gpio_write(RESISTOR_GPIO, LOW);
  bcm2835_gpio_write(COOLER_GPIO, LOW);
  
  bcm2835_close();
}

void config_gpio_outputs() {
  if (!bcm2835_init()) {
    printf("Erro ao configurar os outputs do gpio\n");
    exit(1);
  }

  bcm2835_gpio_fsel(RESISTOR_GPIO, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(COOLER_GPIO, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(TEMPERATURE_SENSOR_GPIO, BCM2835_GPIO_FSEL_OUTP);
}