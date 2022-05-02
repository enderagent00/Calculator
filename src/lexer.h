
#pragma once
#include <stdio.h>
#include "token.h"
#define LEXER_NO_ERROR 0
#define LEXER_ERROR 1

typedef struct {
	char* Source;
	size_t SourceLength;
	char Character;
	int Index;
} Lexer;

Lexer* InitLexer(char* src, size_t srclen);
TokenNode* LexerGenerateTokens(Lexer* lexer);

