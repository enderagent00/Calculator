
/*
parser.c
- Parses an array of tokens into an AST
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "token.h"
#include "string.h"
#include "debug.h"
#define FUNCPTR(name) AstNode*(*name)(Parser* parser)
#define TRUE 1
#define FALSE 0

/* LOCAL ITEMS */

static int ParserError = PARSER_NO_ERROR;

// Ast Nodes

static AstNode* InitNumberNode(Token* token) {
	AstNode* node = calloc(1, sizeof(AstNode));
	node->Option.Number = calloc(1, sizeof(NumberNode));
	node->Option.Number->Token = token;
	node->Type = AST_NODE_NUMBER;

	return node;
}

static AstNode* InitUnaryNode(Token* token, int sign) {
	AstNode* node = calloc(1, sizeof(AstNode));
	node->Option.Number = calloc(1, sizeof(NumberNode));
	node->Option.UnaryNumber->Token = token;
	node->Option.UnaryNumber->Sign = sign;
	node->Type = AST_NODE_UNARY;

	return node;
}

static AstNode* InitBinOpNode(AstNode* left, AstNode* right, Token* op) {
	AstNode* node = calloc(1, sizeof(AstNode));
	node->Option.BinOp = calloc(1, sizeof(BinaryOperatorNode));
	node->Option.BinOp->Left = left;
	node->Option.BinOp->Right = right;
	node->Option.BinOp->Operator = op;
	node->Type = AST_NODE_BINOP;

	return node;
}

// Parser

static AstNode* ParserExpression(Parser* parser);

// Checks if token.Type is in the operator array
static int ValidateTokenType(Token* token, int* ops, size_t opslen) {
	for (int i = 0; i < opslen; i++) {
		if (ops[i] == token->Type) {
			return TRUE;
		}
	}

	return FALSE;
}

static void ParserAdvance(Parser* parser) {
	if (parser->TokenList->Length > parser->Index && parser->Token->Type != TOKEN_EOF) {
		parser->Token = &parser->TokenList->Tokens[++parser->Index];
	}
}

// Returns a number expression
// Handles parenthesis expression
static AstNode* ParserFactor(Parser* parser) {
	Token* token = parser->Token;
	
	if (token->Type == TOKEN_LPAREN) {
		ParserAdvance(parser);
		AstNode* result = ParserExpression(parser);

		if (parser->Token->Type != TOKEN_RPAREN) {
			ParserError = PARSER_ERROR; // Parenthesis was not closed
			return;
		}

		ParserAdvance(parser);
		return result;
	}
	else if (token->Type == TOKEN_PLUS) {
		ParserAdvance(parser);
		AstNode* value = ParserFactor(parser);

		if (value->Type == AST_NODE_NUMBER) {
			return InitUnaryNode(value->Option.UnaryNumber->Token, SIGN_POSITIVE);
		}
		else if (value->Type == AST_NODE_UNARY) {
			if (value->Option.UnaryNumber->Sign == SIGN_POSITIVE) {
				return InitUnaryNode(value->Option.UnaryNumber->Token, SIGN_POSITIVE);
			}
			else if (value->Option.UnaryNumber->Sign == SIGN_NEGATIVE) {
				return InitUnaryNode(value->Option.UnaryNumber->Token, SIGN_NEGATIVE);
			}
		}
	}
	else if (token->Type == TOKEN_MINUS) {
		ParserAdvance(parser);
		AstNode* value = ParserFactor(parser);

		if (value->Type == AST_NODE_NUMBER) {
			return InitUnaryNode(value->Option.UnaryNumber->Token, SIGN_NEGATIVE);
		}
		else if (value->Type == AST_NODE_UNARY) {
			if (value->Option.UnaryNumber->Sign == SIGN_POSITIVE) {
				return InitUnaryNode(value->Option.UnaryNumber->Token, SIGN_NEGATIVE);
			}
			else if (value->Option.UnaryNumber->Sign == SIGN_NEGATIVE) {
				return InitUnaryNode(value->Option.UnaryNumber->Token, SIGN_POSITIVE);
			}
		}
	}

	const int ops2[] = { TOKEN_INT, TOKEN_FLOAT };

	if (ValidateTokenType(token, ops2, 2)) {
		ParserAdvance(parser);
		return InitNumberNode(token);
	}

	// Syntax Error
	ParserError = PARSER_ERROR;
}

// Returns an expression based on the operators
static AstNode* ParserBinOp(Parser* parser, FUNCPTR(function), int* ops, size_t opslen) {
	AstNode* left = function(parser);

	while (ValidateTokenType(parser->Token, ops, opslen)) {
		Token* op = parser->Token;
		ParserAdvance(parser);
		AstNode* right = function(parser);

		left = InitBinOpNode(left, right, op);
	}

	return left;
}

// Looks for a multiply or divide expression
static AstNode* ParserTerm(Parser* parser) {
	const int ops[] = { TOKEN_MULTIPLY, TOKEN_DIVIDE };
	return ParserBinOp(parser, ParserFactor, ops, 2);
}

// Looks for a plus or minus expression
static AstNode* ParserExpression(Parser* parser) {
	const int ops[] = { TOKEN_PLUS, TOKEN_MINUS };
	return ParserBinOp(parser, ParserTerm, ops, 2);
}

/* PUBLIC ITEMS */

// Parser

Parser* InitParser(TokenArray* tokenlist) {
	Parser* parser = calloc(1, sizeof(Parser));
	parser->TokenList = tokenlist;
	parser->Token = &tokenlist->Tokens[0];
	parser->Index = 0;

	return parser;
}

AstNode* ParserGenerateAst(Parser* parser) {
	AstNode* ast = ParserExpression(parser);
	return ParserError ? PARSER_ERROR : ast;
}

// Ast Node

char* AstNodeToString(AstNode* node) {
	if (node->Type == AST_NODE_NUMBER) {
		return TokenToString(node->Option.Number->Token);
	}
	else if (node->Type == AST_NODE_UNARY) {
		char* buffer = AllocateString(255);
		char* tokenstr = TokenToString(node->Option.UnaryNumber->Token);

		sprintf(
			buffer, "%s%s",
			UnarySignToString(node->Option.UnaryNumber->Sign),
			tokenstr
		);

		free(tokenstr);
		return buffer;
	}
	else if (node->Type == AST_NODE_BINOP) {
		char* buffer = AllocateString(255);
		char* tokenstr = TokenToString(node->Option.BinOp->Operator);

		sprintf(
			buffer, "(%s, %s, %s)",
			AstNodeToString(node->Option.BinOp->Left),
			tokenstr,
			AstNodeToString(node->Option.BinOp->Right)
		);

		free(tokenstr);
		return buffer;
	}

	return FillString("NODE TYPE NOT RECOGNISED");
}

const char* UnarySignToString(int sign) {
	switch (sign) {
		case SIGN_POSITIVE:
			return "+";
		case SIGN_NEGATIVE:
			return "-";
		default:
			return "UNRECOGNISED SIGN";
	}
}

char* UnaryNumberToString(UnaryNode* node) {
	const char* signstr = UnarySignToString(node->Sign);
	char* buffer = AllocateString(255);
	
	sprintf(buffer, "%s%s", signstr, node->Token->Value);
	return buffer;
}

