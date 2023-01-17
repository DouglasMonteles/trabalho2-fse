#include <stdio.h>

#include "pid.h"

double saida_medida, sinal_de_controle;
double referencia = 0.0;
double Kp = 0.0;  // Ganho Proporcional
double Ki = 0.0;  // Ganho Integral
double Kd = 0.0;  // Ganho Derivativo
int T = 1.0;      // Período de Amostragem (ms)
unsigned long last_time;
double erro_total, erro_anterior = 0.0;
int sinal_de_controle_MAX = 100.0;
int sinal_de_controle_MIN = -100.0;

void config_pid() {
  short option;

  Kp = Kp_DEFAULT;
  Ki = Ki_DEFAULT;
  Kd = Ki_DEFAULT;  

  printf("\n\nComo deseja inicializar as constantes do PID?\n");
  printf("[1] - Usar valores padrao (Kp=%lf), (Ki=%lf), (Kd=%lf) \n", Kp, Ki, Kd);
  printf("[2] - Informar os valores Kp, Ki e Kd manualmente\n\n");

  printf("Informe o numero da opcao desejada:\n");

  pid_configura_constantes(Kp, Ki, Kd); // default values

  scanf(" %hd", &option);

  if (option == 2) {
    scanf(" %lf %lf %lf", &Kp, &Ki, &Kd);
    // setted values
    pid_configura_constantes(Kp, Ki, Kd);
  } else if (option < 0 || option >= 3) {
    printf("Informe uma opcao valida!\n");
  }
}

void pid_configura_constantes(double Kp_, double Ki_, double Kd_) {
  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_;
}

void pid_atualiza_referencia(float referencia_){
  referencia = (double) referencia_;
}

double pid_controle(double saida_medida) {

  double erro = referencia - saida_medida;

  erro_total += erro; // Acumula o erro (Termo Integral)

  if (erro_total >= sinal_de_controle_MAX) {
    erro_total = sinal_de_controle_MAX;
  } else if (erro_total <= sinal_de_controle_MIN) {
    erro_total = sinal_de_controle_MIN;
  }

  double delta_error = erro - erro_anterior; // Diferença entre os erros (Termo Derivativo)

  sinal_de_controle = Kp*erro + (Ki*T)*erro_total + (Kd/T)*delta_error; // PID calcula sinal de controle

  if (sinal_de_controle >= sinal_de_controle_MAX) {
    sinal_de_controle = sinal_de_controle_MAX;
  } else if (sinal_de_controle <= sinal_de_controle_MIN) {
    sinal_de_controle = sinal_de_controle_MIN;
  }

  erro_anterior = erro;

  return sinal_de_controle;
}