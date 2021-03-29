//
// Created by tallarry on 3/28/21.
//

#ifndef XASM_ENCODING_H
#define XASM_ENCODING_H

std::map<std::string, u16> instructions = {{"mov", 0x0000}, {"add", 0x1000}, {"sub", 0x2000}, {"cmp", 0x3000}, {"and", 0x4000}, {"or", 0x5000}, {"xor", 0x6000},
                                           {"clr", 0x8000}, {"neg", 0x8040}, {"inc", 0x8080}, {"dec", 0x80c0}, {"asl", 0x8100}, {"asr", 0x8140}, {"lsr", 0x8180}, {"rol", 0x81c0},
                                           {"ror", 0x8200}, {"rlc", 0x8240}, {"rrc", 0x8280}, {"jmp", 0x82c0}, {"call", 0x8300}, {"push", 0x8340}, {"pop", 0x8380}
                                           };

#endif //XASM_ENCODING_H
