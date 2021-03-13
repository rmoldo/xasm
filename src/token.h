//
// Created by moldo on 06/03/2021.
//

#ifndef XASM_TOKEN_H
#define XASM_TOKEN_H

enum class TokenType {
    Instruction = 0x0,
    Number,
    Lparan,
    Rparan,
    Colon,
    Dot,
    Comma,
    Register,
    Label,
    NewLine,
    Comment,
    XASMEOF
};

struct Token {
    TokenType type;
    std::string value;
};

#endif //XASM_TOKEN_H
