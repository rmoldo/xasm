//
// Created by tallarry on 3/27/21.
//

#ifndef XASM_XASMGENERATOR_H
#define XASM_XASMGENERATOR_H

#include <vector>
#include "parser.h"

class XASMGenerator : private XASMParser{
public:
    XASMGenerator(Lexer& lexer);
    void generate();

private:
    u16 getRegisterNumber(u16& operand);
    bool operandDest(u16& instruction);
    void operandSrc(u16& instruction);
    void generateObjectCode();
    void parse();

    std::vector<u16> data;
    u16 pc;
    u16 immediateValue;
};


#endif //XASM_XASMGENERATOR_H
