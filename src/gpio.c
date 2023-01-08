#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <bcm2835.h>

#include "gpio.h"

short pin_status(int pin) {
  return bcm2835_gpio_lev(pin);
}

void turn_on_or_off_output(int output) {
  printf("O pin gpio: %d estava: %d\n", output, pin_status(output));

  if (pin_status(output) == HIGH) {
    bcm2835_gpio_write(output, LOW);
  } else {
    bcm2835_gpio_write(output, HIGH);
  }
  
  printf("E agora o pin gpio: %d esta: %d\n", output, pin_status(output));
}

void handle_gpio_interrupt() {
  bcm2835_close();
}

void config_gpio_outputs() {
  if (!bcm2835_init()) {
    printf("Erro ao configurar os outputs do gpio\n");
    exit(1);
  }

  bcm2835_gpio_fsel(RESISTOR_GPIO, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(FAN_GPIO, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(TEMPERATURE_SENSOR_GPIO, BCM2835_GPIO_FSEL_OUTP);
}