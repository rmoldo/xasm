//
// Created by moldo on 13/03/2021.
//

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "parser.h"
#include "defs.h"

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
                if (checkNextToken(TokenType::Colon)) {
                        label();
                        getNextToken();
                }

                // TODO(MOLDO): check if instruction is valid
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
        switch(instructionType(currentToken.value)) {
                case 1:
                        getNextToken();
                        operandDest();
                        match(TokenType::Comma);
                        operandSrc();

                        break;
                case 2:
                        if (currentToken.value == "push" || currentToken.value == "pop") {
                                getNextToken();
                                match(TokenType::Register);
                        }
                        else if (currentToken.value == "call" || currentToken.value == "jmp") {
                                getNextToken();
                                operandSrc();
                        } else {
                                getNextToken();
                                operandDest();
                        }

                        break;
                case 3:
                        getNextToken();
                        match(TokenType::Label);

                        break;
                case 4:
                        if (!checkNextToken(TokenType::NewLine) && !checkNextToken(TokenType::Comment)) {
                                std::cerr << currentToken.value << " does not need operands. \n";
                                std::abort();
                        }

                        getNextToken();

                        break;
                default:
                        std::cerr << currentToken.value << " unknown instruction\n";
                        std::abort();
                        break;
        }
}

void XASMParser::operandDest() {
        if (checkCurrentToken(TokenType::Number)) {
                std::cerr << "Destination can not be an immediate value\n";
                std::abort();
        }

        if (checkCurrentToken(TokenType::Register))
                // Direct addressing
                getNextToken();
        else if (checkCurrentToken(TokenType::Lparan) && checkNextToken(TokenType::Register)) {
                // Indirect adressing
                match(TokenType::Lparan);
                match(TokenType::Register);
                match(TokenType::Rparan);
        } else {
                // Indexed addressing
                match(TokenType::Number);
                match(TokenType::Lparan);
                match(TokenType::Register);
                match(TokenType::Rparan);
        }
}

void XASMParser::operandSrc() {
        if ((checkCurrentToken(TokenType::Register) || checkCurrentToken(TokenType::Number)) && checkNextToken(TokenType::NewLine))
                getNextToken();
        else if (checkCurrentToken(TokenType::Lparan) && checkNextToken(TokenType::Register)) {
                match(TokenType::Lparan);
                match(TokenType::Register);
                match(TokenType::Rparan);
        } else {
                match(TokenType::Number);
                match(TokenType::Lparan);
                match(TokenType::Register);
                match(TokenType::Rparan);
        }
}

int instructionType(const std::string &instruction) {
        auto classB1Iterator = std::find(classB1InstructionVector.begin(), classB1InstructionVector.end(), instruction);
        auto classB2Iterator = std::find(classB2InstructionVector.begin(), classB2InstructionVector.end(), instruction);
        auto classB3Iterator = std::find(classB3InstructionVector.begin(), classB3InstructionVector.end(), instruction);
        auto classB4Iterator = std::find(classB4InstructionVector.begin(), classB4InstructionVector.end(), instruction);

        if (classB1Iterator != classB1InstructionVector.end())
                return 1;

        if (classB2Iterator != classB2InstructionVector.end())
                return 2;

        if (classB3Iterator != classB3InstructionVector.end())
                return 3;

        if (classB4Iterator != classB4InstructionVector.end())
                return 4;

        return -1;
}
