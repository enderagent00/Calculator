
#include <stdio.h>
#include <stdlib.h>
#include "calculator.h"

int main(void) {
	// Example

	char* result = CompileString("(-5 + 0.5) * 12.34");

	printf("Result: %s\n", CompileString(result));
}





