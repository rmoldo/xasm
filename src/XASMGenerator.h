/**
 * Interface for XASMGenerator class
 * @file XASMGenerator.h
 * @author Silvan Talos
 * @version 3/27/21
 */

#ifndef XASM_XASMGENERATOR_H
#define XASM_XASMGENERATOR_H

#include <vector>
#include "parser.h"

class XASMGenerator : private XASMParser {
public:
    /**
     * C-tor
     * @param lexer used for parsing
     * @param map containing addresses of labels, obtained in first pass
     */
    XASMGenerator(Lexer &lexer, Labels &labels);

    ///Generates the binary file containing the object code
    void generate();

private:
    ///Parses operand extracting register number
    u16 getRegisterNumber(u16 &operand);

    ///Adds destination operand to encoding
    bool operandDest(u16 &instruction);

    ///Adds source operand to encoding
    void operandSrc(u16 &instruction);

    ///Generates encoding for an instruction
    void generateObjectCode();

    ///Parses the file extracting each instruction
    void parse();

    ///Verifies label is defined within program
    void checkLabelDefined(std::string label);

    std::vector<u16> data;
    u16 pc;
    u16 immediateValue;
    Labels labels;
};


#endif //XASM_XASMGENERATOR_H
