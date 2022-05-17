
# Calculator

### About
This is an advanced mathematical calculator in C which is able to compute results for mathematical expressions

### How It Works
- Lexer
  - Read in the input string
  - Search through the string to find individual tokens
  - Return the tokens
- Parser
  - Take tokens as an input
  - Parse through the tokens to generate an abstract syntax tree
  - Return the abstract syntax tree
- Interpreter
  - Take an abstract syntax tree as an input
  - Sort through the AST looking for individual expressions
  - Compute the results of all of the expressions
  - Return the result of the full mathematical expression

### Features
- Supports general and simple mathematical expressions
- Supports floating point numbers
- Supports negative numbers
- Supports parenthesis
- Computes result very quickly

### Images

- Very large and complex expression
![Capture](https://user-images.githubusercontent.com/97055625/168841224-90fba1c3-c31f-4520-8d77-5e8457b27749.PNG)
