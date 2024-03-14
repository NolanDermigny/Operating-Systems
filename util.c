#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "util.h"
void builtInError() {
	write(STDERR_FILENO, "An error has occurred\n", strlen("An error has occurred\n")); 
}


