#include <iostream>

#include "./AST/AST.h"
#include "./lexer/lexer.h"
#include "./parser/parser.h"

bool interpret(std::istream& input, std::ostream& output);