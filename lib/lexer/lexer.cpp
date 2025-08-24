#include "lexer.h"

#include <any>
#include <iostream>
#include <string>

Token Lexer::getNextToken() {
    if (endOfFile()) {
        return Token{TokenType::EOF_T, std::string{"EOF"}};
    }

    while (!endOfFile()) {
        // skip comments and whitespaces
        if (curr_ind + 1 < str.size() && str.substr(curr_ind, 2) == "//") {
            skipComments();
            continue;
        }
        if (str[curr_ind] == '\n') {
            advance();
            ++curr_line_;
            line_index_ = 0;
            continue;
            // return Token{TokenType::NEWLINE, '\n'};
        }

        if (str[curr_ind] == ' ') {
            skipWhitespaces();
            continue;
        }

        if (str[curr_ind] == '\"') {
            return getString();
        }

        if (isBoolean()) {
            if (nextStringIs("true")) {
                advance(std::string{"true"}.size());
                return Token{TokenType::BOOLEAN, static_cast<double>(1)};
            }
            advance(std::string{"false"}.size());
            return Token{TokenType::BOOLEAN, static_cast<double>(0)};
        }

        if (std::isdigit(str[curr_ind])) {
            return getNum();
        }

        if (isNull()) {
            advance(std::string{"nil"}.size());
            return Token{TokenType::NIL, NULL};
        }

        if (isNot()) {
            if ((str[curr_ind] == '!')) {
                advance();
            } else {
                advance(std::string{"not"}.size());
            }
            return Token{TokenType::NOT, '!'};
        }

        if (isAnd()) {
            advance(std::string{"and"}.size());
            return Token{TokenType::AND, NULL};
        }

        if (isOr()) {
            advance(std::string{"or"}.size());
            return Token{TokenType::OR, NULL};
        }

        if (nextStringIs("while")) {
            advance(std::string{"while"}.size());
            return Token{TokenType::WHILE, NULL};
        }

        if (nextStringIs("function")) {
            advance(std::string{"function"}.size());
            return Token{TokenType::FUNCTION, std::string{"function"}};
        }

        if (nextStringIs("for")) {
            advance(std::string{"for"}.size());
            return Token{TokenType::FOR, NULL};
        }

        if (nextStringIs("end")) {
            advance(std::string{"end"}.size());
            return Token{TokenType::END, NULL};
        }
        if (nextStringIs("return")) {
            advance(std::string{"return"}.size());
            return Token{TokenType::RETURN, std::string{"return"}};
        }

        if (nextStringIs("break")) {
            advance(std::string{"break"}.size());
            return Token{TokenType::BREAK, NULL};
        }

        if (nextStringIs("continue")) {
            advance(std::string{"continue"}.size());
            return Token{TokenType::CONTINUE, NULL};
        }

        if (nextStringIs("else")) {
            advance(std::string{"else"}.size());
            return Token{TokenType::ELSE, NULL};
        }

        if (nextStringIs("if")) {
            advance(std::string{"if"}.size());
            return Token{TokenType::IF, NULL};
        }

        if (nextStringIs("then")) {
            advance(std::string{"then"}.size());
            return Token{TokenType::THEN_T, NULL};
        }

        if (nextStringIs("in")) {
            advance(std::string{"in"}.size());
            return Token{TokenType::IN, NULL};
        }

        if (std::isalpha(str[curr_ind])) {
            return getVar();
        }

        // find tokens-symbols
        char curr_symb = str[curr_ind];
        advance();
        char next_symb = peek();

        switch (curr_symb) {
            case '+':
                if (next_symb == '=') {
                    advance();
                    return Token{TokenType::ADD_ASSIGNMENT, std::string{"+="}};
                }
                return Token{TokenType::SUM, std::string{"+"}};
            case '-':
                if (next_symb == '=') {
                    advance();
                    return Token{TokenType::SUB_ASSIGNMENT, std::string{"-="}};
                }
                return Token{TokenType::SUB, std::string{"-"}};
            case '*':
                if (next_symb == '=') {
                    advance();
                    return Token{TokenType::MULT_ASSIGNMENT, std::string{"*="}};
                }
                return Token{TokenType::MULT, std::string{"*"}};
            case '/':
                if (next_symb == '=') {
                    advance();
                    return Token{TokenType::DIV_ASSIGNMENT, std::string{"/="}};
                }
                return Token{TokenType::DIV, std::string{"/"}};

            case '=':
                if (next_symb == '=') {
                    advance();
                    return Token{TokenType::EQUAL, std::string{"=="}};
                }
                return Token{TokenType::ASSIGNMENT, std::string{"="}};
            case '!':
                if (next_symb == '=') {
                    advance();
                    return Token{TokenType::NOT_EQUAL, std::string{"!="}};
                }
                break;
            case '^':
                if (next_symb == '=') {
                    advance();
                    return Token{TokenType::POW_ASSIGNMENT, std::string{"^="}};
                }
                return Token{TokenType::POW, std::string{"^"}};
            case '%':
                if (next_symb == '=') {
                    advance();
                    return Token{TokenType::MOD_ASSIGNMENT, std::string{"%="}};
                }
                return Token{TokenType::MOD, std::string{"%"}};
            case '<':
                if (next_symb == '=') {
                    advance();
                    return Token{TokenType::LESS_OR_EQUAL, std::string{"<="}};
                }
                return Token{TokenType::LESS, std::string{"<"}};
            case '>':
                if (next_symb == '=') {
                    advance();
                    return Token{TokenType::MORE_OR_EQUAL, std::string{">="}};
                }
                return Token{TokenType::MORE, std::string{">"}};
            case ',':
                return Token{TokenType::COMMA, ','};
            case '.':
                return Token{TokenType::DOT, '.'};
            case '(':
                return Token{TokenType::LPAREN, '('};
            case ')':
                return Token{TokenType::RPAREN, ')'};
            case '[':
                return Token{TokenType::LBRACKET, '['};
            case ']':
                return Token{TokenType::RBRACKET, ']'};
            case ':':
                return Token{TokenType::COLON, ':'};
            default:
                return Token{TokenType::ERR, std::string{str[curr_ind]}};
                break;
        }
    }
    if (endOfFile()) {
        return Token{TokenType::EOF_T, std::string{"EOF"}};
    }
    return Token{TokenType::ERR, std::string{str[curr_ind]}};
}

bool Lexer::endOfFile() { return curr_ind >= str.size(); }

void Lexer::advance(size_t n) {
    for (int max_ind = std::min(str.size(), curr_ind + n); curr_ind < max_ind; ++curr_ind, ++line_index_) {
    }
}

void Lexer::skipWhitespaces() {
    while (curr_ind < str.size() && str[curr_ind] == ' ') {
        ++curr_ind;
        ++line_index_;
    }
}
void Lexer::skipComments() {
    if (curr_ind < (str.size() - 1) && str.substr(curr_ind, 2) == "//") {
        curr_ind += 2;
        while (curr_ind < str.size() && str[curr_ind] != '\n' && !(isNewLine())) {
            ++curr_ind;
            ++line_index_;
        }
    }
}

Token Lexer::getString() {
    advance();
    std::string tmp;
    while (curr_ind < str.size() && str[curr_ind] != '"') {
        if (str[curr_ind] == '\\') {
            if (curr_ind + 1 < str.size()) {
                switch (str[curr_ind + 1]) {
                    case 't':
                        tmp += '\t';
                        break;
                    case 'n':
                        tmp += '\n';
                        break;
                    default:
                        tmp += str[curr_ind + 1];
                        break;
                }
            } else {
                return Token{TokenType::ERR, NULL};
            }
        }
        tmp += str[curr_ind];
        advance();
    }
    if (curr_ind >= str.size()) {  // not " found
        return Token{TokenType::ERR, NULL};
    }
    advance();
    return Token{TokenType::STRING, tmp};
}

Token Lexer::getNum() {
    std::string num;
    while (curr_ind < str.size() && std::isdigit(str[curr_ind])) {
        num += str[curr_ind];
        advance();
    }
    if (!endOfFile() && (str[curr_ind] == '.' || str[curr_ind] == 'e')) {
        if (str[curr_ind] == '.') {
            num += '.';
            advance();
        }
        if (curr_ind >= str.size() || (!std::isdigit(str[curr_ind]) && (str[curr_ind] != 'e'))) {
            return Token{TokenType::ERR, NULL};
        }
        while (curr_ind < str.size() && std::isdigit(str[curr_ind])) {
            num += str[curr_ind];
            advance();
        }

        // floating point notation
        if (curr_ind < str.size() && str[curr_ind] == 'e') {
            num += 'e';
            advance();
            if (curr_ind < str.size() && (str[curr_ind] == '-' || str[curr_ind] == '+')) {
                num += str[curr_ind];
                advance();
            }
            if (curr_ind >= str.size() || !std::isdigit(str[curr_ind])) {
                return Token{TokenType::ERR, NULL};
            }

            while (curr_ind < str.size() && std::isdigit(str[curr_ind])) {
                num += str[curr_ind];
                advance();
            }
        }
    }

    return Token{TokenType::NUM, std::strtod(num.c_str(), nullptr)};
}

Token Lexer::getVar() {
    std::string var;
    while (curr_ind < str.size() && (std::isalnum(str[curr_ind]) || str[curr_ind] == '_')) {
        var += str[curr_ind];
        advance();
    }
    return Token{TokenType::VAR, var};
}

bool Lexer::isBoolean() { return nextStringIs("true") || nextStringIs("false"); }

bool Lexer::isNull() { return nextStringIs("nil"); }

bool Lexer::isAnd() { return nextStringIs("and"); }

bool Lexer::isNot() {
    return nextStringIs("not") || (str[curr_ind] == '!' && (curr_ind + 1 < str.size()) && str[curr_ind + 1] != '=');
}

bool Lexer::isOr() { return nextStringIs("or") || nextStringIs("||"); }

bool Lexer::isNewLine() { return (str[curr_ind] == '\\' && (curr_ind + 1 < str.size()) && (str[curr_ind + 1] == 'n')); }

bool Lexer::nextStringIs(const std::string& s) {
    if (curr_ind + s.size() - 1 < str.size() && str.substr(curr_ind, s.size()) == s) {
        if (curr_ind + s.size() >= str.size() || (!std::isalpha(str[curr_ind + s.size()]))) {
            return true;
        }
    }
    return false;
}

char Lexer::peek() {
    if (curr_ind < str.size()) {
        return str[curr_ind];
    }
    return 0;
}

size_t Lexer::getPos() const { return curr_ind; }

size_t Lexer::getLineNum() const { return curr_line_; }

size_t Lexer::getSymbLineNum() const { return line_index_; }

std::string Lexer::getCurrString() const {
    size_t end_line_ind = curr_ind;
    size_t start_line_ind = curr_ind;
    if (curr_ind == str.size() || str[curr_ind] == '\n' && curr_ind) {
        start_line_ind = curr_ind - 1;
        while (start_line_ind && str[start_line_ind] != '\n') {
            --start_line_ind;
        }
    }
    while (start_line_ind && str[start_line_ind] != '\n') {
        --start_line_ind;
    }
    while (end_line_ind < str.size() && str[end_line_ind] != '\n') {
        ++end_line_ind;
    }
    return str.substr(start_line_ind, end_line_ind - start_line_ind);
}