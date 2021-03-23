//
// Created by moldo on 13/03/2021.
//

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "parser.h"
#include "defs.h"
#include "verifier.h"

std::vector<std::string> Verifier::classB1InstructionVector {"mov", "add", "sub", "cmp", "and", "or", "xor"};

std::vector<std::string> Verifier::classB2InstructionVector {"clr", "neg", "inc", "dec", "asl", "asr", "lsr", "rol", "ror", "rlc", "rrc", "jmp",
                                                             "call", "push", "pop"};

std::vector<std::string> Verifier::classB3InstructionVector {"br", "bne", "beq", "bpl", "bcs", "bcc", "bvs", "bvc"};

std::vector<std::string> Verifier::classB4InstructionVector {"clc", "clv", "clz", "cls", "ccc", "sec", "sev", "sez", "ses", "scc", "nop", "ret",
                                                             "reti","halt", "wait", "pushflag", "popflag", "pushpc", "poppc"};

// TODO(Moldo): Find another solution for this
std::vector<std::string> tokenTypeString {"Instruction", "Number", "Lparan", "Rparan", "Colon", "Dot",
                                          "Comma", "Register", "Label", "NewLine", "Comment", "XASMEOF"};

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
        if (!checkCurrentToken(type))
                throw std::runtime_error("Expected " + tokenTypeString[(int)type] + " but found " + currentToken.value);

        if (type == TokenType::Register && !Verifier::matchRegister(currentToken.value))
                throw std::runtime_error(currentToken.value + " is not a register.\n");
        else if (type == TokenType::Number && !Verifier::matchInteger(currentToken.value))
                throw std::runtime_error(currentToken.value + " is not an integer.\n");

        getNextToken();
}

void XASMParser::parse() {
        while (!checkCurrentToken(TokenType::XASMEOF)) {
                // TODO: Get address of label
                if (checkNextToken(TokenType::Colon)) {
                        label();
                        getNextToken();
                }

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
                        if (!checkNextToken(TokenType::NewLine) && !checkNextToken(TokenType::Comment))
                                throw std::runtime_error(currentToken.value + " does not need operands");

                        getNextToken();

                        break;
                default:
                        throw std::runtime_error(currentToken.value + " unknown instruction.\n");
        }
}

void XASMParser::operandDest() {
        if (checkCurrentToken(TokenType::Number) && checkNextToken(TokenType::Comma))
                throw std::runtime_error("Destination can not be an immediate value.\n");

        if (checkCurrentToken(TokenType::Register))
                // Direct addressing
                match(TokenType::Register);
        else if (checkCurrentToken(TokenType::Lparan) && checkNextToken(TokenType::Register)) {
                // Indirect addressing
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
        if (checkCurrentToken(TokenType::Register) && (checkNextToken(TokenType::NewLine) || checkNextToken(TokenType::Comment)))
                match(TokenType::Register);
        else if (checkCurrentToken(TokenType::Number) && (checkNextToken(TokenType::NewLine) || checkNextToken(TokenType::Comment)))
                match(TokenType::Number);
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
        if (Verifier::checkClassB1(instruction))
                return 1;

        if (Verifier::checkClassB2(instruction))
                return 2;

        if (Verifier::checkClassB3(instruction))
                return 3;

        if (Verifier::checkClassB4(instruction))
                return 4;

        return -1;
}
