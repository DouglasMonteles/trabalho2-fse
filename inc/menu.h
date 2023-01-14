#ifndef _MENU_H_
#define _MENU_H_

#define MENU_OPTIONS 10
#define MENU_LENGTH_OPTIONS 100
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
  "[9] - Enviar estado do sistema [1] - Ligado / [0] - Desligado",
  "[10] - Sair",
};

short menu();
void handle_menu_option(short option);

#endif