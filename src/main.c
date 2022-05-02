
#include <stdio.h>
#include <stdlib.h>
#include "calculator.h"

int main(void) {
	char* s1 = "5 + 5";
	char* s2 = "(-5 + 10) * 12";
	char* s3 = "-4543 + -0.4 * 3";
	char* s4 = "(5433 * 54) + (555 / 3) * ----------++---944 + -0.8";

	printf("Result: %s\n", CompileString(s1));
	printf("Result: %s\n", CompileString(s2));
	printf("Result: %s\n", CompileString(s3));
	printf("Result: %s\n", CompileString(s4));
}





