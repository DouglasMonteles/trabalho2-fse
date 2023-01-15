#ifndef _MENU_H_
#define _MENU_H_

#define MENU_OPTIONS 13
#define MENU_LENGTH_OPTIONS 200
#define EXIT MENU_OPTIONS

static const unsigned char menu_options[MENU_OPTIONS][MENU_LENGTH_OPTIONS] = {
  "[1] - Ligar/Desligar Resistor",
  "[2] - Ligar/Desligar Ventoinha",
  "[3] - Sensor de Temperatura",
  "[4] - Solicita temperatura interna do forno",
  "[5] - Solicita temperatura de referencia do forno",
  "[6] - Le comandos do usuario",
  "[7] - Enviar sinal de controle",
  "[8] - Enviar sinal de referencia",
  "[9] - Enviar estado do sistema: [1] - Ligado | [0] - Desligado",
  "[10] - Enviar modo de controle da temperatura de referencia: [0] - Dashboard | [1] - Curva/Terminal",
  "[11] - Enviar o estado de funcionamento: [0] - Parado | [1] - Funcionando",
  "[12] - Enviar temperatura ambiente",
  "[13] - Sair",
};

short menu();
void handle_menu_option(short option);
void handle_user_command(int command);

#endif