#ifndef _GPIO_H_
#define _GPIO_H_

#define RESISTOR_GPIO 23
#define FAN_GPIO 24
#define TEMPERATURE_SENSOR_GPIO 4

short pin_status(int pin);
void turn_on_or_off_output(int output);
void handle_gpio_interrupt();
void config_gpio_outputs();

#endif