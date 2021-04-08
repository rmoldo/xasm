//
// Created by moldo on 23/03/2021.
//

#include "verifier.h"

#include <string>
#include <regex>
#include <algorithm>

bool Verifier::matchRegister(const std::string &reg) {
        // Regex for register matching
        std::regex register_regex {"^r([0-9]|1[0-5])$"};

        return std::regex_match(reg, register_regex);
}

bool Verifier::matchInteger(const std::string &number) {
        // Regex for integer numbers matching
        std::regex integer_regex {"^[-]?[1-9]\\d*$|^0$"};

        return std::regex_match(number, integer_regex);
}

bool Verifier::checkClassB1(const std::string &instruction) {
         return std::find(classB1InstructionVector.begin(), classB1InstructionVector.end(), instruction) !=
                classB1InstructionVector.end();
}

bool Verifier::checkClassB2(const std::string &instruction) {
        return std::find(classB2InstructionVector.begin(), classB2InstructionVector.end(), instruction) !=
               classB2InstructionVector.end();
}

bool Verifier::checkClassB3(const std::string &instruction) {
        return std::find(classB3InstructionVector.begin(), classB3InstructionVector.end(), instruction) !=
               classB3InstructionVector.end();
}

bool Verifier::checkClassB4(const std::string &instruction) {
        return std::find(classB4InstructionVector.begin(), classB4InstructionVector.end(), instruction) !=
               classB4InstructionVector.end();
}
