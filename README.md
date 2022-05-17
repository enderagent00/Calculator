# Calculator

### About
This is an advanced mathematical calculator in C which is able to compute results for mathematical expressions

### How it Works
- The program takes an input string and throws it through a lexer
  - The lexer is responsible for finding the individual tokens in a string
- The program passes tokens to the parser
  - The parser is responsible for turning a token array into an abstract syntax tree
- The program passes the abstract syntax tree to the interpreter
  - The interpreter reads in an abstract syntax tree and computes the result of the expression

### Features
- Supports general and simple mathematical expressions
- Supports floating point numbers
- Supports negative numbers
- Supports parenthesis
- Is very fast
