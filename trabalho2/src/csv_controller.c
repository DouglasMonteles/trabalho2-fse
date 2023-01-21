#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "csv_controller.h"
#include "debug.h"

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

int obtain_temperature_curve_from_csv(int* time_in_seconds, float* temperature) {
  char row[100];
  char* character;
  int counter = 0;
  
  FILE *csv_curve_file = fopen(CURVA_REFLOW_FILE_PATH, READ_FILE_ONLY);

  while (feof(csv_curve_file) != 1) {
    // Read the first row and do nothing
    if (counter == 0)
      fgets(row, 100, csv_curve_file);

    fgets(row, 100, csv_curve_file);
    character = strtok(row, ",");

    int counter_char = 0;

    while (character != NULL) {
      if (counter_char == 0) {  // Time param
        time_in_seconds[counter] = atoi(character);
      } else if (counter_char == 1) {                  // Temperature param
        temperature[counter] = atof(character);
      }

      character = strtok(NULL, ",");
      counter_char++;
    }
    
    #ifndef IS_DEBUG_ENABLED
      printf("Dado lido: %d %f\n", time_in_seconds[counter], temperature[counter]);
    #endif
    counter++;
  }

  fclose(csv_curve_file);
  return counter;
}
