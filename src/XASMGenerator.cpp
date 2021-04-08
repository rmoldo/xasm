/**
 * Object code generator
 * @file XASMGenerator.h
 * @author Silvan Talos
 * @version 3/27/21
 */

#include <iostream>
#include <fstream>
#include "XASMGenerator.h"
#include "encoding.h"

XASMGenerator::XASMGenerator(Lexer &lexer, Labels &labels) : XASMParser{lexer}, labels{labels}, pc{0} {}

void XASMGenerator::generate() {
        try {
                parse();

                std::ofstream file = std::ofstream("output.out", std::ios::binary);
                //writes all content of data to file as binary
                file.write((char *) &data[0], data.size() * sizeof(u16));
                file.close();
        } catch (std::exception &e) {
                throw;
        } catch (...) {
                std::cerr << "Unknown error during generate stage" << std::endl;
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

                        //note if destination operand needs an immediate value to be added after instruction in data vector
                        bool destNeedsImmediate = operandDest(instruction);
                        match(TokenType::Comma);
                        //processes source operand and saves instruction in data vector
                        operandSrc(instruction);

                        //add immediate value in data vector after instruction
                        if (destNeedsImmediate)
                                data.push_back(immediateValue);

                        break;
                }

                case 2: {
                        u16 instruction = instructions[crtToken.value];

                        //call and jmp can also have a label as offset
                        if (crtToken.value == "call" || crtToken.value == "jmp") {
                                if(checkNextToken(TokenType::Label)) {
                                        getNextToken();

                                        data.push_back(instruction);

                                        checkLabelDefined(getCurrentToken().value);
                                        //add the label address as immediate value
                                        data.push_back(labels[getCurrentToken().value]);

                                        match(TokenType::Label);
                                        //increment pc as label address is immediate value stored at next location
                                        pc += 2;

                                        break;
                                }
                        }

                        getNextToken();
                        bool needsImmediate = operandDest(instruction);
                        data.push_back(instruction);

                        if (needsImmediate)
                                data.push_back(immediateValue);

                        break;
                }

                case 3: {
                        u16 instruction = instructions[crtToken.value];
                        getNextToken();

                        checkLabelDefined(getCurrentToken().value);

                        //calculate relative offset
                        int dest = labels[getCurrentToken().value] - (pc + 2);
                        instruction |= (dest & 0xFF);
                        data.push_back(instruction);

                        match(TokenType::Label);

                        break;
                }

                case 4: {
                        u16 instruction = instructions[crtToken.value];
                        data.push_back(instruction);
                        getNextToken();

                        break;
                }

                default:
                        throw std::runtime_error(crtToken.value + " unknown instruction.\n");
        }

        pc += 2;
}

bool XASMGenerator::operandDest(u16 &instruction) {
        if (checkCurrentToken(TokenType::Register)) {
                // Direct addressing
                u16 mad = 0x0010;
                instruction |= (getRegisterNumber(mad) & 0x1F);

                match(TokenType::Register);
        } else if (checkCurrentToken(TokenType::Lparan) && checkNextToken(TokenType::Register)) {
                // Indirect addressing
                match(TokenType::Lparan);

                u16 mad = 0x0020;
                instruction |= (getRegisterNumber(mad) & 0x2F);

                match(TokenType::Register);
                match(TokenType::Rparan);
        } else {
                // Indexed addressing
                //save immediate value to be added after instruction
                immediateValue = static_cast<u16>(std::stoul(getCurrentToken().value));

                match(TokenType::Number);
                match(TokenType::Lparan);
                
                u16 mad = 0x0030;
                instruction |= (getRegisterNumber(mad) & 0x3F);
                
                match(TokenType::Register);
                match(TokenType::Rparan);

                //increment pc for the immediate value of the index
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
        } catch (...) {
                throw std::runtime_error("Couldn't convert to number");
        }

        return operand;
}

void XASMGenerator::operandSrc(u16 &instruction) {
        if (checkCurrentToken(TokenType::Register) &&
            (checkNextToken(TokenType::NewLine) || checkNextToken(TokenType::Comment))) {
                //Direct addressing
                u16 mas = 0x0010;
                instruction |= ((getRegisterNumber(mas) & 0x1F) << 6);

                match(TokenType::Register);
        } else if (checkCurrentToken(TokenType::Number) &&
                   (checkNextToken(TokenType::NewLine) || checkNextToken(TokenType::Comment))) {
                //Immediate addressing
                u16 immediateVal{static_cast<u16>(std::stoul(getCurrentToken().value))};

                match(TokenType::Number);
                
                pc += 2;
                
                data.push_back(instruction);
                data.push_back(immediateVal);
                
                return;
        } else if (checkCurrentToken(TokenType::Lparan) && checkNextToken(TokenType::Register)) {
                //Indirect addressing
                match(TokenType::Lparan);

                u16 mas = 0x0020;
                instruction |= ((getRegisterNumber(mas) & 0x2F) << 6);

                match(TokenType::Register);
                match(TokenType::Rparan);
        } else {
                //Indexed addressing
                u16 immediateVal{static_cast<u16>(std::stoul(getCurrentToken().value))};
                match(TokenType::Number);
                match(TokenType::Lparan);

                u16 mas = 0x0030;
                instruction |= ((getRegisterNumber(mas) & 0x3F) << 6);

                match(TokenType::Register);
                match(TokenType::Rparan);

                pc += 2;
                
                data.push_back(instruction);
                data.push_back(immediateVal);
                
                return;
        }
        
        data.push_back(instruction);
}

void XASMGenerator::checkLabelDefined(std::string label) {
        auto searchedLabel = labels.find(label);
        if(searchedLabel == labels.end())
                throw std::runtime_error("Label " + label + " not defined\n");
}
