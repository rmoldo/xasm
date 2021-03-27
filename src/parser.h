//
// Created by moldo on 13/03/2021.
//

#ifndef XASM_PARSER_H
#define XASM_PARSER_H

#include <map>

#include "lexer.h"

class XASMParser {
public:
    XASMParser(Lexer &lexer);

    bool checkCurrentToken(TokenType type) const;
    bool checkNextToken(TokenType type) const;

    void getNextToken();
    void match(TokenType type);

    void parse();
    void label();
    void instruction();
    void operandDest();
    void operandSrc();

private:
    Lexer lexer;
    Token currentToken;
    Token nextToken;
    std::map<std::string, uint> labels;
    uint pc;
};

int instructionType(const std::string &instruction);

#endif //XASM_PARSER_H
