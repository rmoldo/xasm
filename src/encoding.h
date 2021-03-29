//
// Created by tallarry on 3/28/21.
//

#ifndef XASM_ENCODING_H
#define XASM_ENCODING_H

std::map<std::string, u16> instructions = {{"mov", 0x0000}, {"add", 0x1000}, {"sub", 0x2000}, {"cmp", 0x3000}, {"and", 0x4000}, {"or", 0x5000}, {"xor", 0x6000}};

#endif //XASM_ENCODING_H
