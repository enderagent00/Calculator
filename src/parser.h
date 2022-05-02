
#pragma once
#include "token.h"
#define PARSER_NO_ERROR 0
#define PARSER_ERROR 1

typedef struct {
	TokenArray* TokenList;
	Token* Token;
	int Index;
} Parser;

struct AstNode_t;

typedef struct {
	Token* Token;
} NumberNode;

typedef struct {
	Token* Token;

	enum {
		SIGN_POSITIVE,
		SIGN_NEGATIVE
	} Sign;
} UnaryNode;

typedef struct {
	struct AstNode_t* Left;
	struct AstNode_t* Right;
	Token* Operator;
} BinaryOperatorNode;

typedef struct AstNode_t {
	enum {
		AST_NODE_NUMBER,
		AST_NODE_UNARY,
		AST_NODE_BINOP
	} Type;

	union {
		NumberNode* Number;
		UnaryNode* UnaryNumber;
		BinaryOperatorNode* BinOp;
	} Option;
} AstNode;

Parser* InitParser(TokenNode* tokens);
AstNode* ParserGenerateAst(Parser* parser);
char* AstNodeToString(AstNode* node);
const char* UnarySignToString(int sign);
char* UnaryNumberToString(UnaryNode* node);
