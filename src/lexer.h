//
// Created by moldo on 06/03/2021.
//

#ifndef XASM_LEXER_H
#define XASM_LEXER_H

#include <string>
#include "token.h"

class Lexer {
public:
    Lexer() = default;
    Lexer(std::string &source);

    Token nextToken();

    void rewind();
private:
    std::string source;
    char currentChar;
    int position;

    void nextChar();
    char peek();
    bool isSpace() const;
    bool isNewLine() const;
    void skipSpaces();
};

#endif //XASM_LEXER_H
