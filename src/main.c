#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "main.h"

int main(int argc, char **argv) {
  signal(SIGINT, handle_interrupt);

  

  return 0;
}

void handle_interrupt(int signal) {
  exit(0);
}