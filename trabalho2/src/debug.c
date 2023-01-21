#include <stdio.h>

#include "debug.h"

void log_info(char* message) {
  #ifndef IS_DEBUG_ENABLED
    printf("[Info]: %s\n", message);
  #endif
}

void log_warn(char* message) {
  #ifndef IS_DEBUG_ENABLED
    printf("[Warn]: %s\n", message);
  #endif
}

void log_error(char* message) {
  #ifndef IS_DEBUG_ENABLED
    printf("[Error]: %s\n", message);
  #endif
}