#pragma once

#include <any>
#include <iostream>
#include <string>

#include "token.h"

class Lexer {
   public:
    Lexer(const std::string& s) : str(s), curr_ind(0), curr_line_(0) {}
    Lexer(std::string&& s) : str(std::move(s)), curr_ind(0), curr_line_(0) {}
    Token getNextToken();
    size_t getPos() const;
    size_t getLineNum() const;
    size_t getSymbLineNum() const;

    std::string getCurrString() const;

   private:
    bool endOfFile();

    void advance(size_t n = 1);

    void skipWhitespaces();
    void skipComments();
    bool isNewLine();

    Token getString();

    Token getNum();
    Token getVar();
    bool isBoolean();
    bool isNull();

    bool isAnd();
    bool isNot();
    bool isOr();

    bool nextStringIs(const std::string& s);
    char peek();

    size_t curr_line_;
    size_t line_index_;
    size_t curr_ind;
    std::string str;
};
