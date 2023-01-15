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
  printf("Interrompendo o programa e finalizando os processos abertos...\n");
  
  handle_gpio_interrupt();

  printf("Todos os processos utilizados pelo programa foram finalizados com sucesso\n");
  exit(0);
}