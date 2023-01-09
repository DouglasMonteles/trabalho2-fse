#ifndef _MENU_H_
#define _MENU_H_

#define MENU_OPTIONS 5
#define MENU_LENGTH_OPTIONS 50
#define EXIT MENU_OPTIONS

static const unsigned char menu_options[MENU_OPTIONS][MENU_LENGTH_OPTIONS] = {
  "[1] - Ligar/Desligar Resistor",
  "[2] - Ligar/Desligar Ventoinha",
  "[3] - Sensor de Temperatura",
  "[4] - Solicita temperatura interna do forno",
  "[5] - Sair",
};

short menu();
void handle_menu_option(short option);

#endif