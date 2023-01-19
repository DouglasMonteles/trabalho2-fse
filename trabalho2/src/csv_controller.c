#include <stdio.h>
#include <time.h>
#include <string.h>

#include "csv_controller.h"

struct tm * get_instant_data() {
  time_t instant;
  time(&instant);

  return localtime(&instant);
}

void register_system_logs_in_csv(logger_system_data data) {
  FILE *log_csv_file = fopen(SYSTEM_LOGS_FILE_PATH, CREATE_IF_NOT_EXISTS_AND_WHITE);
  
  struct tm *instant_data;
  instant_data = get_instant_data();

  fprintf(log_csv_file, "%02d/%02d/%02d,", instant_data->tm_mday, (instant_data->tm_mon + 1), (instant_data->tm_year % 100 + 2000));
  fprintf(log_csv_file, "%02d:%02d:%02d,", instant_data->tm_hour, instant_data->tm_min, instant_data->tm_sec);
  fprintf(log_csv_file, "%.2lf,", data.intern_temperature);
  fprintf(log_csv_file, "%.2lf,", data.external_temperature);
  fprintf(log_csv_file, "%.2lf,", data.user_defined_temperature);
  fprintf(log_csv_file, "%.2lf,", data.resistor_active_percent);
  fprintf(log_csv_file, "%.2lf,", data.cooler_active_percent);
  fprintf(log_csv_file, "\n");
  fclose(log_csv_file);
}