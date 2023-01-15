#include <stdio.h>
#include <unistd.h>

#include "menu.h"
#include "gpio.h"
#include "uart_modbus.h"

short menu() {
  short selected_option = 0;

  printf("\n\n==================== MENU DE CONTROLE =========================\n");

  for (int i = 0; i < MENU_OPTIONS; i++) {
    printf(menu_options[i]);
    printf("\n");
  }

  printf("==================== =============== =========================\n\n");

  printf("\nInforme o numero da opcao desejada:\n");
  scanf("%d", &selected_option);

  printf("Voce selecionou a opcao: %s\n\n", menu_options[selected_option-1]);
  return selected_option;
}

void handle_menu_option(short option) {
  switch (option) {
  case 1: 
    turn_on_or_off_output(RESISTOR_GPIO);
    break;

  case 2: 
    turn_on_or_off_output(COOLER_GPIO);
    break;
  
  case 4: {
    float intern_temperature = request_float_intern_temperature_message();
    printf("Temperatura interna recebida: %f\n", intern_temperature);
    
    break;
  }

  case 5: {
    float reference_temperature = request_float_reference_temperature_message();
    printf("Temperatura de referencia recebida: %f\n", reference_temperature);
    
    break;
  }

  case 6: {
    int command = read_user_commands();
    printf("Le comandos do usuario: %d\n", command);

    handle_user_command(command);
    
    break;
  }

  case 7: {
    int controller_sign = 13;
    send_controller_sign(controller_sign);
    break;
  }

  case 8: {
    float reference_sign = 2;
    send_reference_sign(reference_sign);
    break;
  }

  case 9: {
    char state = 0;

    printf("Informe o valor do estado do sistema: [1] - Ligado | [0] - Desligado\n");
    scanf(" %d", &state);

    int system_state = send_system_state(state);
    printf("O sistema esta: %s\n", (system_state == 0) ? "desligado" : "ligado");
    break;
  }

  case 10: {
    char controller_mode = 0;

    printf("Informe o modo de controle da temperatura de referencia: [0] - Dashboard | [1] - Curva/Terminal\n");
    scanf(" %d", &controller_mode);
    
    int mode = send_controller_mode(controller_mode);
    printf("Modo de controle: %s\n", (mode == 0) ? "Dashboard" : "Curva/Terminal");
    break;
  }

  case 11: {
    char working_status = 0;

    printf("Informe o estado de funcionamento: [0] - Parado | [1] - Funcionando\n");
    scanf(" %d", &working_status);
    
    int status = send_working_status(working_status);
    printf("Estado de funcionamento: %s\n", (status == 0) ? "Parado" : "Funcionando");
    break;
  }

  case 12: {
    float room_temperature = 0.f;
    
    printf("Informe a temperatura ambiente: ");
    scanf(" %f", &room_temperature);

    float temperature = send_room_temperature(room_temperature);

    printf("Temperatura ambiente: %f\n", room_temperature);
    break;
  }

  default:
    break;
  }

  printf("\n\n");
}

void handle_user_command(int command) {
  switch (command) {
    case 161:
      printf("Liga o Forno\n");
      break;

    case 162:
      printf("Desliga o Forno\n");
      break;

    case 163:
      printf("Inicia aquecimento\n");
      break;

    case 164:
      printf("Cancela processo\n");
      break;

    case 165:
      printf("Menu : alterna entre o modo de Temperatura de Referência e Curva de Temperatura\n");
      break;

    default:
      printf("UNKNOWN\n");
  }
}