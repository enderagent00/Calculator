
#pragma once
#include "parser.h"
#define INTERPRETER_NO_ERROR 0
#define INTERPRETER_ERROR

typedef struct {
	AstNode* Ast;
} Interpreter;

typedef struct {
	union {
		int Integer;
		double Float;
	} Value;

	int Type;
} Number;

Interpreter* InitInterpreter(AstNode* ast);
Number* InterpreterComputeResult(Interpreter* interpreter);





