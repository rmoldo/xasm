//
// Created by moldo on 06/03/2021.
//


#include <string>
#include <algorithm>
#include "lexer.h"
#include "defs.h"

#define XASMEOFConstant 3

Lexer::Lexer(std::string &source) {
        this->source = source;
        position = 0;
        currentChar = 0;

        // Convert source to lowercase
        for (char &c : this->source)
                if (std::isupper(c))
                        c = (char)std::tolower(c);

        // Initialize currentChar with first character in source
        nextChar();
}

Token Lexer::nextToken() {
        Token t {};

        skipSpaces();

        t.value += currentChar;

        switch(currentChar) {
                case '\r': case '\n': case '\f':
                        t.type = TokenType::NewLine;

                        while (isNewLine() || isSpace())
                                nextChar();

                        break;
                case '(':
                        t.type = TokenType::Lparan;
                        nextChar();

                        break;
                case ')':
                        t.type = TokenType::Rparan;
                        nextChar();

                        break;
                case '.':
                        t.type = TokenType::Dot;
                        nextChar();

                        break;
                case ':':
                        t.type = TokenType::Colon;
                        nextChar();

                        break;
                case ',':
                        t.type = TokenType::Comma;
                        nextChar();

                        break;
                case ';':
                        t.type = TokenType::Comment;
                        nextChar();

                        while(!isNewLine()) {
                                t.value += currentChar;
                                nextChar();
                        }

                        break;
                case '0' ... '9':
                case '-':
                        t.type = TokenType::Number;
                        nextChar();

                        while (std::isdigit(currentChar)) {
                                t.value += currentChar;
                                nextChar();
                        }

                        break;
                case XASMEOFConstant: case 0:
                        t.type = TokenType::XASMEOF;
                        nextChar();

                        break;
                default:
                        // Register
                        if (currentChar == '$') {
                                t.type = TokenType::Register;
                                // Delete $ from register name
                                t.value = "";

                                nextChar();

                                while(std::isalnum(currentChar)) {
                                        t.value += currentChar;
                                        nextChar();
                                }
                        } else {
                                t.type = TokenType::Instruction;
                                nextChar();

                                while(std::isalnum(currentChar)) {
                                        t.value += currentChar;
                                        nextChar();
                                }

                                std::string word = t.value;

                                auto iterator = std::find_if(instructionVector.begin(),
                                                             instructionVector.end(),
                                                             [&word](const std::string &instructionName) {
                                                                 return word == instructionName;
                                                             });

                                if (currentChar == ':' || iterator == instructionVector.end())
                                        t.type = TokenType::Label;
                        }

                        break;
        }

        return t;
}

void Lexer::nextChar() {
        if (position >= source.size())
                currentChar = XASMEOFConstant;
        else
                currentChar = source[position];

        position++;
}

char Lexer::peek() {
        // Check if peek is out of size
        if (position + 1 >= source.size())
                return 0;

        return source[position + 1];
}

bool Lexer::isNewLine() const{
       return currentChar == '\n' || currentChar == '\r' || currentChar == '\f';
}

bool Lexer::isSpace() const {
       return currentChar == ' '  || currentChar == '\t' || currentChar == '\v';
}

void Lexer::skipSpaces() {
        while(isSpace())
                nextChar();
}

void Lexer::rewind() {
        position = 0;
        nextChar();
}
