#ifndef __DEBUG__
#define __DEBUG__

#define IS_DEBUG_ENABLED 1

void log_info(char* message);
void log_warn(char* message);
void log_error(char* message);

#endif