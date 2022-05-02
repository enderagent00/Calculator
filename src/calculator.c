
/*
calculator.c
- Mathematical calculator written in C
- Created by enderagent
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"
#include "parser.h"
#include "lexer.h"
#include "token.h"
#include "string.h"
#include "debug.h"
#define SUCCESS 1
#define FAILURE 0

// Evaluates mathematical expression from an input string
int CompileString(char* src, char* dst) {
	size_t srclen = strlen(src);

	// Lexer

	Lexer* lexer = InitLexer(src, srclen);
	TokenNode* tokens = LexerGenerateTokens(lexer);
	free(lexer);

	if (tokens == LEXER_ERROR) {
		PRINTLINE_ERROR("Error in lexer, aborting\n");
		return FAILURE;
	}

	// Parser

	TokenArray* tokenarr = TokenNodeToArray(tokens, 1);
	Parser* parser = InitParser(tokenarr);
	AstNode* ast = ParserGenerateAst(parser);

	free(tokenarr);
	free(parser);

	if (ast == PARSER_ERROR) {
		PRINTLINE_ERROR("Error in parser, aborting\n");
		return FAILURE;
	}

	// Interpreter

	Interpreter* interpreter = InitInterpreter(ast);
	Number* result = InterpreterComputeResult(interpreter);

	free(interpreter);

	if (result == INTERPRETER_ERROR) {
		free(result);
		PRINTLINE_ERROR("Error in interpreting, aborting\n");
		return FAILURE;
	}

	if (result->Type == TOKEN_INT) {
		sprintf(dst, "%d", result->Value.Integer);
	}
	else if (result->Type == TOKEN_FLOAT) {
		sprintf(dst, "%g", result->Value.Float);
	}

	free(result);
	return SUCCESS;
}





