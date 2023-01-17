#ifndef PID_H_
#define PID_H_

#define Kp_DEFAULT 30.0;  // Ganho Proporcional
#define Ki_DEFAULT 0.2;  // Ganho Integral
#define Kd_DEFAULT 400.0;  // Ganho Derivativo

void config_pid();
void pid_configura_constantes(double Kp_, double Ki_, double Kd_);
void pid_atualiza_referencia(float referencia_);
double pid_controle(double saida_medida);

#endif /* PID_H_ */