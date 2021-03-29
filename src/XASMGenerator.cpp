//
// Created by tallarry on 3/27/21.
//

#include <iostream>
#include <fstream>
#include "XASMGenerator.h"
#include "encoding.h"

XASMGenerator::XASMGenerator(Lexer &lexer) : XASMParser{lexer}, pc{0} {}

void XASMGenerator::generate() {
    try {
        parse();
        std::ofstream file = std::ofstream("output.out", std::ios::binary);
        file.write((char *) &data[0], data.size() * sizeof(u16));
        file.close();
    }
    catch (std::exception &e) {
        std::cerr << "Error during generate stage parsing: " << e.what();
        throw;
    }
}

void XASMGenerator::parse() {
    while (!checkCurrentToken(TokenType::XASMEOF)) {

        if (checkCurrentToken(TokenType::Instruction))
            generateObjectCode();

        getNextToken();
    }
}

void XASMGenerator::generateObjectCode() {
    Token crtToken{getCurrentToken()};
    switch (instructionType(crtToken.value)) {
        case 1: {
            u16 instruction = instructions[crtToken.value];
            getNextToken();

            bool destNeedsImmediate = operandDest(instruction);
            //std::cout << std::hex << instruction << " immediate: " << immediateValue <<std::endl;
            match(TokenType::Comma);
            operandSrc(instruction);
            //std::cout << std::hex << instruction << " immediate: " << immediateValue <<std::endl;
            if (destNeedsImmediate)
                data.push_back(immediateValue);
            break;
        }
        case 2:
            if (crtToken.value == "call" || crtToken.value == "jmp") {
                getNextToken();
                //TODO
                //operandSrc();
            } else {
                u16 instruction = instructions[crtToken.value];
                getNextToken();
                bool needsImmediate = operandDest(instruction);
                data.push_back(instruction);
                if (needsImmediate)
                    data.push_back(immediateValue);
            }

            break;
        case 3:
            getNextToken();
            //calculate label offset
            std::cout << "found label " << crtToken.value << " at address: " << pc << std::endl;
            match(TokenType::Label);

            break;
        case 4:
            if (!checkNextToken(TokenType::NewLine) && !checkNextToken(TokenType::Comment))
                throw std::runtime_error(crtToken.value + " does not need operands");

            getNextToken();

            break;
        default:
            throw std::runtime_error(crtToken.value + " unknown instruction.\n");
    }
    pc += 2;


    for (auto nr : data)
        std::cout << std::hex << nr << " ";
}

bool XASMGenerator::operandDest(u16 &instruction) {
    if (checkCurrentToken(TokenType::Register)) {
        // Direct addressing
        u16 mad {0x0010};
        instruction |= (getRegisterNumber(mad) & 0x1F);
        match(TokenType::Register);
    } else if (checkCurrentToken(TokenType::Lparan) && checkNextToken(TokenType::Register)) {
        // Indirect addressing
        match(TokenType::Lparan);
        u16 mad {0x0020};
        instruction |= (getRegisterNumber(mad) & 0x2F);
        match(TokenType::Register);
        match(TokenType::Rparan);
    } else {
        // Indexed addressing
        immediateValue = static_cast<u16>(std::stoul(getCurrentToken().value));
        match(TokenType::Number);
        match(TokenType::Lparan);
        u16 mad {0x0030};
        instruction |= (getRegisterNumber(mad) & 0x3F);
        match(TokenType::Register);
        match(TokenType::Rparan);
        pc += 2;
        return true;
    }
    return false;
}

u16 XASMGenerator::getRegisterNumber(u16 &operand) {
    std::string reg{getCurrentToken().value};
    reg = reg.substr(1);
    try {
        operand |= static_cast<u16>(std::stoul(reg));
    }
    catch (...) {
        throw std::runtime_error("Couldn't convert to number");
    }
    return operand;
}

void XASMGenerator::operandSrc(u16 &instruction) {
    if (checkCurrentToken(TokenType::Register) && (checkNextToken(TokenType::NewLine) || checkNextToken(TokenType::Comment))) {
        u16 mad {0x0010};
        instruction |= ((getRegisterNumber(mad) & 0x1F) << 6);
        match(TokenType::Register);
    }
    else if (checkCurrentToken(TokenType::Number) && (checkNextToken(TokenType::NewLine) || checkNextToken(TokenType::Comment))) {
        u16 immediateVal {static_cast<u16>(std::stoul(getCurrentToken().value))};
        match(TokenType::Number);
        pc += 2;
        data.push_back(instruction);
        data.push_back(immediateVal);
        return;
    }
    else if (checkCurrentToken(TokenType::Lparan) && checkNextToken(TokenType::Register)) {
        match(TokenType::Lparan);
        u16 mad {0x0020};
        instruction |= ((getRegisterNumber(mad) & 0x2F) << 6);
        match(TokenType::Register);
        match(TokenType::Rparan);
    } else {
        u16 immediateVal {static_cast<u16>(std::stoul(getCurrentToken().value))};
        match(TokenType::Number);
        match(TokenType::Lparan);
        u16 mad {0x0030};
        instruction |= ((getRegisterNumber(mad) & 0x3F) << 6);
        match(TokenType::Register);
        match(TokenType::Rparan);
        pc += 2;
        data.push_back(instruction);
        data.push_back(immediateVal);
        return;
    }
    data.push_back(instruction);
}
