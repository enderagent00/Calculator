
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "token.h"

// Token

Token* InitToken(char* value, int type) {
	Token* token = calloc(1, sizeof(Token));
	token->Value = value;
	token->Type = type;

	return token;
}

const char* TokenTypeToString(int type) {
	switch (type) {
		case TOKEN_PLUS:
			return "PLUS";
		case TOKEN_MINUS:
			return "MINUS";
		case TOKEN_MULTIPLY:
			return "MULTIPLY";
		case TOKEN_DIVIDE:
			return "DIVIDE";
		case TOKEN_LPAREN:
			return "LPAREN";
		case TOKEN_RPAREN:
			return "RPAREN";
		case TOKEN_INT:
			return "INT";
		case TOKEN_FLOAT:
			return "FLOAT";
		case TOKEN_EOF:
			return "EOF";
		default:
			return "INVALID";
	}
}

char* TokenToString(Token* token) {
	const char* typestr = TokenTypeToString(token->Type);
	char* buffer = malloc(sizeof(char) * 255);

	sprintf(buffer, "[%s:%s]", typestr, token->Value);
	return buffer;
}

// Token Node

TokenNode* InitTokenNode(Token* value) {
	TokenNode* node = calloc(1, sizeof(TokenNode));
	node->Value = value;
	node->Next = NO_NODE;

	return node;
}

void AppendTokenNode(TokenNode* node, Token* value) {
	while (node == NO_NODE || node->Next != NO_NODE) {
		node = node->Next;
	}

	node->Next = InitTokenNode(value);
}

void UnallocateTokenNode(TokenNode* node) {
	TokenNode* temp;

	while (node != NO_NODE) {
		temp = node;
		node = node->Next;
		free(temp);
	}
}

// Token Array

TokenArray* TokenNodeToArray(TokenNode* node, int unallocateNode) {
	size_t length = 0;
	TokenNode* temp = node;

	while (temp != NO_NODE) {
		length++;
		temp = temp->Next;
	}

	TokenArray* output = calloc(1, sizeof(TokenArray));
	output->Tokens = calloc(length, sizeof(Token));
	output->Length = length;
	int index = 0;

	while (node != NO_NODE) {
		output->Tokens[index++] = *node->Value;
		node = node->Next;
	}

	if (unallocateNode) {
		UnallocateTokenNode(node);
	}

	return output;
}






