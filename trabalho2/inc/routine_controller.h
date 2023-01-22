#ifndef _ROUTINE_CONTROLLER_H_
#define _ROUTINE_CONTROLLER_H_

#define DEFAULT_TEMPERATURE 20
#define IS_CSV_CURVE_FILE 1
#define IS_MANUAL 0

void handle_terminal_process();
void handle_temperature_curve_process();
void handle_dashboard_process();
void handle_potentiometer_process(short is_csv_curve);

#endif