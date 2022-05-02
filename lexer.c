
/*
lexer.c
- Turns an input string into an array of tokens
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "token.h"
#define IsCharNumeric(character) character == '.' || isdigit(character)

static int LexerError = LEXER_NO_ERROR;

// Local

static void LexerAdvance(Lexer* lexer) {
	if (lexer->SourceLength > lexer->Index && lexer->Character != TT_EOF) {
		lexer->Character = lexer->Source[++lexer->Index];
	}
}

static Token* LexerAdvanceCurrent(Lexer* lexer, int type) {
	char* value = calloc(2, sizeof(char));
	value[0] = lexer->Character;
	value[1] = '\0';

	Token* token = InitToken(value, type);
	LexerAdvance(lexer);
	return token;
}

// Parses through lexer.Source and returns the current number
static Token* LexerBuildNumber(Lexer* lexer) {
	char* buffer = calloc(1, sizeof(char) * MAX_TOKEN_VALUE_LEN);
	int decimals = 0;
	int index = 0;

	while (IsCharNumeric(lexer->Character)) {
		if (lexer->Character == '.' && ++decimals > 1) {
			break;
		}
		
		buffer[index++] = lexer->Character;
		LexerAdvance(lexer);
	}

	buffer[index] = '\0';
	return InitToken(buffer, decimals > 0 ? TOKEN_FLOAT : TOKEN_INT);
}

// Skips all whitespace characters
static void LexerSkipWhitespace(Lexer* lexer) {
	while (isspace(lexer->Character)) {
		LexerAdvance(lexer);
	}
}

static Token* LexerGetNextToken(Lexer* lexer) {
	if (lexer->Character != TT_EOF) {
		LexerSkipWhitespace(lexer);

		if (IsCharNumeric(lexer->Character)) {
			return LexerBuildNumber(lexer);
		}

		switch (lexer->Character) {
			case TT_PLUS:
				return LexerAdvanceCurrent(lexer, TOKEN_PLUS);
			case TT_MINUS:
				return LexerAdvanceCurrent(lexer, TOKEN_MINUS);
			case TT_MULTIPLY:
				return LexerAdvanceCurrent(lexer, TOKEN_MULTIPLY);
			case TT_DIVIDE:
				return LexerAdvanceCurrent(lexer, TOKEN_DIVIDE);
			case TT_LPAREN:
				return LexerAdvanceCurrent(lexer, TOKEN_LPAREN);
			case TT_RPAREN:
				return LexerAdvanceCurrent(lexer, TOKEN_RPAREN);
			default:
				printf("[LEXER] Unrecognised Token: '%c'", lexer->Character);
				LexerError = LEXER_ERROR;
				break;
		}
	}

	return InitToken(TT_EOF, TOKEN_EOF);
}

// Public

Lexer* InitLexer(char* src, size_t srclen) {
	Lexer* lexer = calloc(1, sizeof(Lexer));
	lexer->Source = src;
	lexer->SourceLength = srclen;
	lexer->Character = src[0];
	lexer->Index = 0;
	
	return lexer;
}

TokenNode* LexerGenerateTokens(Lexer* lexer) {
	LexerError = LEXER_NO_ERROR;
	TokenNode* node = 0;
	Token* token = 0;
	int index = 0;

	while ((token = LexerGetNextToken(lexer))->Type != TOKEN_EOF) {	
		if (node == NO_NODE) {
			node = InitTokenNode(token);
		}
		else {
			AppendTokenNode(node, token);
		}
	}

	if (LexerError) {
		return LEXER_ERROR;
	}

	AppendTokenNode(node, InitToken(TT_EOF, TOKEN_EOF));
	return node;
}







