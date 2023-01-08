#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "main.h"
#include "gpio.h"
#include "menu.h"

int main(int argc, char **argv) {
  short option;
  signal(SIGINT, handle_interrupt);

  init_application_configs();

  while ((option = menu()) != EXIT) {
    handle_menu_option(option);
  }

  return 0;
}

void init_application_configs() {
  config_gpio_outputs();
}

void handle_interrupt(int signal) {
  handle_gpio_interrupt();
  exit(0);
}