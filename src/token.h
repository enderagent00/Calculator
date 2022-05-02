
#pragma once
#define TT_PLUS '+'
#define TT_MINUS '-'
#define TT_MULTIPLY '*'
#define TT_DIVIDE '/'
#define TT_LPAREN '('
#define TT_RPAREN ')'
#define TT_EOF '\0'
#define MAX_TOKEN_VALUE_LEN 255
#define NO_NODE 0

typedef struct {
	char* Value;

	enum {
		TOKEN_PLUS,
		TOKEN_MINUS,
		TOKEN_MULTIPLY,
		TOKEN_DIVIDE,
		TOKEN_LPAREN,
		TOKEN_RPAREN,
		TOKEN_INT,
		TOKEN_FLOAT,
		TOKEN_EOF
	} Type;
} Token;

Token* InitToken(char* value, int type);
const char* TokenTypeToString(int type);
char* TokenToString(Token* token);

typedef struct node {
	Token* Value;
	struct node* Next;
} TokenNode;

TokenNode* InitTokenNode(Token* value);
void AppendTokenNode(TokenNode* node, Token* value);
void UnallocateTokenNode(TokenNode* node);

typedef struct {
	Token* Tokens;
	size_t Length;
} TokenArray;

TokenArray* TokenNodeToArray(TokenNode* nod, int unallocateNode);
