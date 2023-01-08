#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "main.h"
#include "gpio.h"

int main(int argc, char **argv) {
  signal(SIGINT, handle_interrupt);
  init_application_configs();

  turn_on_or_off_output(RESISTOR_GPIO);

  return 0;
}

void init_application_configs() {
  config_gpio_inputs();
}

void handle_interrupt(int signal) {
  handle_gpio_interrupt();
  exit(0);
}