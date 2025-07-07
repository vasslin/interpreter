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

bool interpret(std::istream& input, std::ostream& output) {
    try {
        auto str = getString(input);
        Lexer lex{str};
        // std::cout << str[112] << std::endl;
        Parser parser{lex};
        parser.setOutputStream(&output);
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