
#include <stdio.h>
#include <stdlib.h>
#include "calculator.h"

int main(void) {
	// Example

	char* expression = "5 + 5 * 2";
	char result[255];

	if (CompileString(expression, result)) {
		printf("Result: %s\n", result);
	}
	else {
		printf("Failed\n");
	}
}





