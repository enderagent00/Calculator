
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"
#include "parser.h"
#include "lexer.h"
#include "token.h"
#include "string.h"
#include "debug.h"

char* CompileString(char* src) {
	size_t srclen = strlen(src);

	// Lexer

	Lexer* lexer = InitLexer(src, srclen);
	TokenNode* tokens = LexerGenerateTokens(lexer);
	free(lexer);

	if (tokens == LEXER_ERROR) {
		PRINTLINE_ERROR("Error in lexer, aborting\n");
		return;
	}

	// Parser

	TokenArray* tokenarr = TokenNodeToArray(tokens, 1);
	Parser* parser = InitParser(tokenarr);
	AstNode* ast = ParserGenerateAst(parser);

	if (ast == PARSER_ERROR) {
		PRINTLINE_ERROR("Error in parser, aborting\n");
		return;
	}

	// Interpreter

	Interpreter* interpreter = InitInterpreter(ast);
	Number* result = InterpreterComputeResult(interpreter);
	char* buffer = AllocateString(255);

	if (result->Type == TOKEN_INT) {
		sprintf(buffer, "%d", result->Value.Integer);
	}
	else if (result->Type == TOKEN_FLOAT) {
		sprintf(buffer, "%g", result->Value.Float);
	}

	free(interpreter);
	return buffer;
}





