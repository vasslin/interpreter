#include "interpreter.h"

std::string getString(std::istream& input) {
    std::string tmp = "";
    std::string s = "";
    while (std::getline(input, tmp)) {
        s += tmp;
        s += '\n';
    }
    return s;
}

bool interpret(std::istream& input) {
    try {
        auto str = getString(input);
        Lexer lex{str};
        Parser parser{lex};
        AST ast{};
        if (parser.parse(ast)) {
            ast.execute();
        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}