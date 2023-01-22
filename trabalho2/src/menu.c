#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "menu.h"
#include "gpio.h"
#include "pid.h"
#include "uart_modbus.h"
#include "bme280_temperature.h"
#include "routine_controller.h"
#include "debug.h"

int is_temperature_manual_or_curve = 0;

short handle_initial_menu() {
  short option;

  printf("Escolha o modo de operacao da aplicacao:\n");
  printf("[1] - Modo Terminal\n");
  printf("[2] - Modo UART\n");
  printf("[Ctrl+c] - Finalizar programa\n");

  scanf(" %hd", &option);
  return option;
}

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
    turn_on_output(RESISTOR_GPIO);
    turn_off_output(COOLER_GPIO);
    break;

  case 2: 
    turn_off_output(RESISTOR_GPIO);
    turn_on_output(COOLER_GPIO);
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

  case 13: {
    init_bme280();
    double temperature;
    int result = get_temperature(&temperature);

    if (result == BME280_RESPONSE_SUCCESS) {
      printf("Temperatura: %lf\n", temperature);
    } else {
      printf("Erro na leitura do bme280\n");
      exit(1);
    }

    break;
  }

  default:
    break;
  }

  printf("\n\n");
}

void handle_user_command(int command) {
  printf("Comando: %d\n", command);
  
  switch (command) {
    case 161:
      printf("Liga o Forno\n");
      send_system_state(1);
      printf("Forno LIGADO\n\n");
      break;

    case 162:
      printf("Desliga o Forno\n");
      send_system_state(0);
      send_working_status(0);
      printf("Forno DESLIGADO\n\n");
      break;

    case 163:
      printf("Inicia aquecimento\n");
      send_working_status(1);
      handle_potentiometer_process(is_temperature_manual_or_curve);
      printf("Aquecimento INICIADO\n\n");
      break;

    case 164:
      printf("Cancela processo\n");
      send_working_status(0);
      printf("Aquecimento CANCELADO\n\n");
      break;

    case 165: { // option 165
      printf("Menu: alterna entre o modo de Temperatura de Referência e Curva de Temperatura\n");
      
      char controller_mode = (is_temperature_manual_or_curve == 0) ? 1 : 0;
      int mode = send_controller_mode(controller_mode);

      is_temperature_manual_or_curve = mode;

      printf("Modo de controle: %s\n", (mode == 0) ? "Dashboard" : "Curva/Terminal");

      if (mode == 0) {
        handle_potentiometer_process(mode);
      } else if (mode == 1) {
        send_controller_mode(1);      // Controle via curva / terminal
      
        short terminal_option = 1;

        printf("[1] - Informar manualmente uma temperatura de referencia.\n");
        printf("[2] - Utilizar o arquivo de curva (csv) para definir a temperatura (graus Celsius) em um dado tempo (segundos)\n");

        scanf(" %hd", &terminal_option);

        if (terminal_option == 1) {
          handle_terminal_process();
        } else if (terminal_option == 2) {
          printf("O arquivo de curva de temperatura sera utilizado.\n");
          handle_temperature_curve_process();
        } else {
          printf("Opcao invalida!\n");
        }
      }
      break;
    }

    default:
      printf("Aguardando o usuario informar um comando...\n");
  }
}