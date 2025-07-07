#pragma once
#include <any>
#include <iostream>

enum class TokenType {
    // numbers
    NUM,
    BOOLEAN,

    STRING,

    // arrays
    LPAREN,    // '('
    RPAREN,    // ')'
    LBRACKET,  // '['
    RBRACKET,  // ']'

    // nulltype
    NIL,

    // operators: arithmetic
    SUM,
    SUB,
    MULT,
    DIV,
    MOD,
    POW,
    // operators: comparison
    EQUAL,
    NOT_EQUAL,
    MORE,
    LESS,
    MORE_OR_EQUAL,
    LESS_OR_EQUAL,

    // operators: logical
    AND,
    OR,
    NOT,

    // operators: assignment
    ASSIGNMENT,
    ADD_ASSIGNMENT,
    SUB_ASSIGNMENT,
    MULT_ASSIGNMENT,
    DIV_ASSIGNMENT,
    MOD_ASSIGNMENT,
    POW_ASSIGNMENT,

    // keywords
    FUNCTION,
    WHILE,
    FOR,
    END,
    BREAK,
    CONTINUE,
    VAR,
    IN,
    EOF_T,
    IF,
    THEN_T,
    ELSE,
    RETURN,

    // other
    QUOTATION,
    DOT,
    COMMA,
    COLON,
    NEWLINE,

    ERR

};

struct Token {
    TokenType type;
    std::any value;
    template <typename T>
    Token(TokenType t, T&& val) : type(t), value(std::forward<T>(val)) {}
};