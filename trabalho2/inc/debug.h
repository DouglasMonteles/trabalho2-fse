#ifndef __DEBUG__
#define __DEBUG__

#define IS_DEBUG_ENABLED 0

void log_info(char* message);
void log_warn(char* message);
void log_error(char* message);

#endif