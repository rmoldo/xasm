//
// Created by moldo on 23/03/2021.
//

#ifndef XASM_VERIFIER_H
#define XASM_VERIFIER_H

#include <vector>
#include <string>

class Verifier {
    static std::vector<std::string> classB1InstructionVector;

    static std::vector<std::string> classB2InstructionVector;

    static std::vector<std::string> classB3InstructionVector;

    static std::vector<std::string> classB4InstructionVector;
public:
    Verifier() = default;
    static bool matchRegister(const std::string &reg);
    static bool matchInteger(const std::string &number);

    static bool checkClassB1(const std::string &instruction);
    static bool checkClassB2(const std::string &instruction);
    static bool checkClassB3(const std::string &instruction);
    static bool checkClassB4(const std::string &instruction);
};

#endif //XASM_VERIFIER_H
