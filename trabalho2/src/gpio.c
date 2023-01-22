#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wiringPi.h>
#include <softPwm.h>

#include "gpio.h"
#include "debug.h"

void turn_on_output(int output) {
  digitalWrite(output, HIGH);
}

void turn_off_output(int output) {
  digitalWrite(output, LOW);
}

char* get_gpio_name(int pin) {
  switch (pin) {
    case RESISTOR_GPIO:
      return "RESISTOR";
    
    case COOLER_GPIO:
      return "VENTOINHA";

    default:
      return "UNKNOWN";
  }
}

void handle_gpio_interrupt() {
  // shutdown the resistor and cooler
  softPwmWrite(RESISTOR_GPIO, 0);
  softPwmWrite(COOLER_GPIO, 0);

  digitalWrite(RESISTOR_GPIO, LOW);
  digitalWrite(COOLER_GPIO, LOW);
  printf("GPIO finalizada com sucesso!\n");
}

void handle_temperature_power(double temp) {
  int temp_power = (int) temp;
  
  if (temp_power < 0) {
    temp_power *= -1;
    
    if(temp_power < 40) 
      temp_power = 40;
    
    softPwmWrite(COOLER_GPIO, temp_power);
    softPwmWrite(RESISTOR_GPIO, 0);
  } else {
    softPwmWrite(RESISTOR_GPIO, temp_power);
    softPwmWrite(COOLER_GPIO, 0);
  }
}

void config_gpio_outputs() {
  if (wiringPiSetup() == -1) {
    log_error("Erro ao inicializar a wiringPi.");
    exit(1);
  }

  pinMode(RESISTOR_GPIO, PWM_OUTPUT);
  pinMode(COOLER_GPIO, PWM_OUTPUT);

  softPwmCreate(RESISTOR_GPIO, 1, 100);
  softPwmCreate(COOLER_GPIO, 1, 100);

  softPwmWrite(RESISTOR_GPIO, 0);
  softPwmWrite(COOLER_GPIO, 0);

  // bcm2835_gpio_fsel(RESISTOR_GPIO, BCM2835_GPIO_FSEL_OUTP);
  // bcm2835_gpio_fsel(COOLER_GPIO, BCM2835_GPIO_FSEL_OUTP);
  // bcm2835_gpio_fsel(TEMPERATURE_SENSOR_GPIO, BCM2835_GPIO_FSEL_OUTP);
}