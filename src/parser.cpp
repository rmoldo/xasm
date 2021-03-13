//
// Created by moldo on 13/03/2021.
//

#include <iostream>
#include <vector>
#include <string>

#include "parser.h"

// TODO(Moldo): Find another solution for this
std::vector<std::string> tokenTypeString {"Instruction",
                                          "Number",
                                          "Lparan",
                                          "Rparan",
                                          "Colon",
                                          "Dot",
                                          "Comma",
                                          "Register",
                                          "Label",
                                          "NewLine",
                                          "Comment",
                                          "XASMEOF"};
XASMParser::XASMParser(Lexer &lexer) {
        this->lexer = lexer;

        // Initialize currentToken and nextToken
        getNextToken();
        getNextToken();
}

bool XASMParser::checkCurrentToken(TokenType type) const {
        return currentToken.type == type;
}

bool XASMParser::checkNextToken(TokenType type) const {
        return nextToken.type == type;
}

void XASMParser::getNextToken() {
        currentToken = nextToken;
        nextToken = lexer.nextToken();
}

void XASMParser::match(TokenType type) {
        if (!checkCurrentToken(type)) {
                std::cerr << "Expected " << tokenTypeString[(int)type] << " but found " << currentToken.value << "\n";
                std::abort();
        }

        getNextToken();
}

void XASMParser::parse() {
        while (!checkCurrentToken(TokenType::XASMEOF)) {
                // TODO(Moldo): get address of label
                if (checkNextToken(TokenType::Colon))
                        label();

                if (checkCurrentToken(TokenType::Instruction))
                        instruction();

                if (checkCurrentToken(TokenType::Comment))
                        getNextToken();

                match(TokenType::NewLine);
        }
}

void XASMParser::label() {
        match(TokenType::Label);
        getNextToken();
}

void XASMParser::instruction() {

}

void XASMParser::operand() {

}
