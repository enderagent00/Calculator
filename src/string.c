
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>

char* AllocateString(size_t length) {
	char* buffer = (char*)malloc(sizeof(char) * length + 1);
	buffer[length] = '\0';

	return buffer;
}

char* FillString(const char* value) {
	char* buffer = AllocateString(strlen(value));
	strcpy(buffer, value);
	return buffer;
}
