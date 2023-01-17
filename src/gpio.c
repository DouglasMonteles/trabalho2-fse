#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wiringPi.h>
#include <softPwm.h>

#include "gpio.h"

short pin_status(int pin) {
  return digitalRead(pin);
}

void turn_on_or_off_output(int output) {
  printf("O(A) %s estava: %s\n", get_gpio_name(output), pin_status(output) == LOW ? "DESLIGADO(A)" : "LIGADO(a)");

  if (pin_status(output) == HIGH) {
    digitalWrite(output, LOW);
  } else {
    digitalWrite(output, HIGH);
  }
  
  printf("O(A) %s agora esta: %s\n", get_gpio_name(output), pin_status(output) == LOW ? "DESLIGADO(A)" : "LIGADO(A)");
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
}

void handle_temperature_power(double temp_power) {
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
  wiringPiSetup();

  pinMode(RESISTOR_GPIO, OUTPUT);
  pinMode(COOLER_GPIO, OUTPUT);

  softPwmCreate(RESISTOR_GPIO, 1, 100);
  softPwmCreate(COOLER_GPIO, 1, 100);

  // bcm2835_gpio_fsel(RESISTOR_GPIO, BCM2835_GPIO_FSEL_OUTP);
  // bcm2835_gpio_fsel(COOLER_GPIO, BCM2835_GPIO_FSEL_OUTP);
  // bcm2835_gpio_fsel(TEMPERATURE_SENSOR_GPIO, BCM2835_GPIO_FSEL_OUTP);
}