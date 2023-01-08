#ifndef _MENU_H_
#define _MENU_H_

#define MENU_OPTIONS 4
#define MENU_LENGTH_OPTIONS 50
#define EXIT 4

static const unsigned char menu_options[MENU_OPTIONS][MENU_LENGTH_OPTIONS] = {
  "[1] - Ligar/Desligar Resistor",
  "[2] - Ligar/Desligar Ventoinha",
  "[3] - Ligar/Desligar Ventoinha",
  "[4] - Sair",
};

short menu();
void handle_menu_option(short option);

#endif