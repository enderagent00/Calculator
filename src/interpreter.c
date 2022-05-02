
/*
interpreter.c
- Computes the result of an AST 
*/

#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "parser.h"

/* LOCAL ITEMS */

static int InterpreterError = INTERPRETER_NO_ERROR;

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

// Perform addition on 2 specified nodes
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

// Perform subtraction on 2 specified nodes
static NumberNode* SubtractNodes(Number* left, Number* right) {
	if (left->Type == TOKEN_INT) {
		if (right->Type == TOKEN_INT) {
			return InitNumberInt(left->Value.Integer - right->Value.Integer);
		}
		else if (right->Type == TOKEN_FLOAT) {
			return InitNumberFloat(left->Value.Integer - right->Value.Float);
		}
	}
	else if (left->Type == TOKEN_FLOAT) {
		if (right->Type == TOKEN_INT) {
			return InitNumberFloat(left->Value.Float - right->Value.Integer);
		}
		else if (right->Type == TOKEN_FLOAT) {
			return InitNumberFloat(left->Value.Float - right->Value.Float);
		}
	}
}

// Perform multiplication on 2 specified nodes
static Number* MultiplyNodes(Number* left, Number* right) {
	if (left->Type == TOKEN_INT) {
		if (right->Type == TOKEN_INT) {
			return InitNumberInt(left->Value.Integer * right->Value.Integer);
		}
		else if (right->Type == TOKEN_FLOAT) {
			return InitNumberFloat(left->Value.Integer * right->Value.Float);
		}
	}
	else if (left->Type == TOKEN_FLOAT) {
		if (right->Type == TOKEN_INT) {
			return InitNumberFloat(left->Value.Float * right->Value.Integer);
		}
		else if (right->Type == TOKEN_FLOAT) {
			return InitNumberFloat(left->Value.Float * right->Value.Float);
		}
	}
}

// Perform division on 2 specified nodes
static Number* DivideNodes(Number* left, Number* right) {
	if (left->Type == TOKEN_INT) {
		if (right->Type == TOKEN_INT) {
			return InitNumberInt(left->Value.Integer / right->Value.Integer);
		}
		else if (right->Type == TOKEN_FLOAT) {
			return InitNumberFloat(left->Value.Integer / right->Value.Float);
		}
	}
	else if (left->Type == TOKEN_FLOAT) {
		if (right->Type == TOKEN_INT) {
			return InitNumberFloat(left->Value.Float / right->Value.Integer);
		}
		else if (right->Type == TOKEN_FLOAT) {
			return InitNumberFloat(left->Value.Float / right->Value.Float);
		}
	}
}

// Interpreter

static Number* InterpreterVisit(Interpreter* interpreter, AstNode* node);

// Returns a Number from an AstNode
static Number* InterpreterVisitNumber(Interpreter* interpreter, AstNode* node) {
	if (node->Type == AST_NODE_NUMBER) {
		NumberNode* number = node->Option.Number;
		free(node);

		return InitNumber(number->Token->Value, number->Token->Type);
	}
	else if (node->Type == AST_NODE_UNARY) {
		UnaryNode* number = node->Option.UnaryNumber;
		free(node);

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

// Returns the result of 2 Numbers based on the AstNode operator
static Number* InterpreterVisitBinOp(Interpreter* interpreter, AstNode* node) {
	Number* left = InterpreterVisit(interpreter, node->Option.BinOp->Left);
	Number* right = InterpreterVisit(interpreter, node->Option.BinOp->Right);
	Number* result = 0;

	switch (node->Option.BinOp->Operator->Type) {
		case TOKEN_PLUS:
			result = AddNodes(left, right);
			break;
		case TOKEN_MINUS:
			result = SubtractNodes(left, right);
			break;
		case TOKEN_MULTIPLY:
			result = MultiplyNodes(left, right);
			break;
		case TOKEN_DIVIDE:
			result = DivideNodes(left, right);
			break;
	}

	if (result == 0) {
		InterpreterError = INTERPRETER_ERROR;
	}
	
	free(left);
	free(right);
	return result;
}

// Decides if it needs to visit a number or find out the result of 2 numbers
static Number* InterpreterVisit(Interpreter* interpreter, AstNode* node) {
	if (node->Type == AST_NODE_NUMBER || node->Type == AST_NODE_UNARY) {
		return InterpreterVisitNumber(interpreter, node);
	}
	else if (node->Type == AST_NODE_BINOP) {
		return InterpreterVisitBinOp(interpreter, node);
	}

	// Unrecognised AST type
	InterpreterError = INTERPRETER_ERROR;
}

/* PUBLIC ITEMS */

Interpreter* InitInterpreter(AstNode* ast) {
	Interpreter* interpreter = calloc(1, sizeof(Interpreter));
	interpreter->Ast = ast;

	return interpreter;
}

Number* InterpreterComputeResult(Interpreter* interpreter) {
	InterpreterError = INTERPRETER_NO_ERROR;
	Number* result = InterpreterVisit(interpreter, interpreter->Ast);

	if (InterpreterError) {
		return INTERPRETER_ERROR;
	}

	return result;
}



