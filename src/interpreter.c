
#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "parser.h"

/* LOCAL ITEMS */

// Number

static Number* InitNumberInt(int value) {
	Number* number = calloc(1, sizeof(Number));
	number->Value.Integer = value;
	number->Type = TOKEN_INT;

	return number;
}

static Number* InitNumberFloat(double value) {
	Number* number = calloc(1, sizeof(Number));
	number->Value.Float = value;
	number->Type = TOKEN_FLOAT;

	return number;
}

static Number* InitNumber(char* value, int type) {
	if (type == TOKEN_INT) {
		int _value = atoi(value);
		return InitNumberInt(_value, type);
	}
	else if (type == TOKEN_FLOAT) {
		double _value = atof(value);
		return InitNumberFloat(_value, type);
	}
}

// Nodes

static Number* AddNodes(Number* left, Number* right) {
	if (left->Type == TOKEN_INT) {
		if (right->Type == TOKEN_INT) {
			return InitNumberInt(left->Value.Integer + right->Value.Integer);
		}
		else if (right->Type == TOKEN_FLOAT) {
			return InitNumberFloat(left->Value.Integer + right->Value.Float);
		}
	}
	else if (left->Type == TOKEN_FLOAT) {
		if (right->Type == TOKEN_INT) {
			return InitNumberFloat(left->Value.Float + right->Value.Integer);
		}
		else if (right->Type == TOKEN_FLOAT) {
			return InitNumberFloat(left->Value.Float + right->Value.Float);
		}
	}
}

static NumberNode* SubtractNodes(Number* left, Number* right) {
	return InitNumberInt(left->Value.Integer - right->Value.Integer);
}

static Number* MultiplyNodes(Number* left, Number* right) {
	return InitNumberInt(left->Value.Integer * right->Value.Integer);
}

static Number* DivideNodes(Number* left, Number* right) {
	return InitNumberInt(left->Value.Integer / right->Value.Integer);
}

// Interpreter

static Number* InterpreterVisit(Interpreter* interpreter, AstNode* node);

static Number* InterpreterVisitNumber(Interpreter* interpreter, AstNode* node) {
	if (node->Type == AST_NODE_NUMBER) {
		NumberNode* number = node->Option.Number;
		return InitNumber(number->Token->Value, number->Token->Type);
	}
	else if (node->Type == AST_NODE_UNARY) {
		UnaryNode* number = node->Option.UnaryNumber;

		if (number->Sign == SIGN_POSITIVE) {
			return InitNumber(number->Token->Value, number->Token->Type);
		}
		else if (number->Sign == SIGN_NEGATIVE) {
			char* value = UnaryNumberToString(number);
			Number* result = InitNumber(value, number->Token->Type);

			free(value);
			return result;
		}
	}
}

static Number* InterpreterVisitBinOp(Interpreter* interpreter, AstNode* node) {
	Number* left = InterpreterVisit(interpreter, node->Option.BinOp->Left);
	Number* right = InterpreterVisit(interpreter, node->Option.BinOp->Right);

	switch (node->Option.BinOp->Operator->Type) {
		case TOKEN_PLUS:
			return AddNodes(left, right);
		case TOKEN_MINUS:
			return SubtractNodes(left, right);
		case TOKEN_MULTIPLY:
			return MultiplyNodes(left, right);
		case TOKEN_DIVIDE:
			return DivideNodes(left, right);
	}
}

static Number* InterpreterVisit(Interpreter* interpreter, AstNode* node) {
	if (node->Type == AST_NODE_NUMBER || node->Type == AST_NODE_UNARY) {
		return InterpreterVisitNumber(interpreter, node);
	}
	else if (node->Type == AST_NODE_BINOP) {
		return InterpreterVisitBinOp(interpreter, node);
	}

	// Error 
}

/* PUBLIC ITEMS */

Interpreter* InitInterpreter(AstNode* ast) {
	Interpreter* interpreter = calloc(1, sizeof(Interpreter));
	interpreter->Ast = ast;

	return interpreter;
}

Number* InterpreterComputeResult(Interpreter* interpreter) {
	return InterpreterVisit(interpreter, interpreter->Ast);
}



