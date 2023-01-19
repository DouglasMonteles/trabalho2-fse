#ifndef _CSV_CONTROLLER_H_
#define _CSV_CONTROLLER_H_

#include <time.h>

#define CURVA_REFLOW_FILE_PATH "curva_reflow.csv"
#define SYSTEM_LOGS_FILE_PATH "system_logs.csv"
#define CREATE_IF_NOT_EXISTS_AND_WHITE "a+"

typedef struct logger_system_data {
  double intern_temperature;
  double external_temperature;
  double user_defined_temperature;
  double resistor_active_percent;
  double cooler_active_percent;
} logger_system_data;

void register_system_logs_in_csv(logger_system_data data);
struct tm * get_instant_data();

#endif