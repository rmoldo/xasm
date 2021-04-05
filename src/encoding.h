/**
 * Contains all instructions and their opcode defined
 * Unknown fields are set to 0
 * @file encoding.h
 * @author Silvan Talos
 * @version 3/28/21
 */

#ifndef XASM_ENCODING_H
#define XASM_ENCODING_H

std::map<std::string, u16> instructions = {{"mov",      0x0000},
                                           {"add",      0x1000},
                                           {"sub",      0x2000},
                                           {"cmp",      0x3000},
                                           {"and",      0x4000},
                                           {"or",       0x5000},
                                           {"xor",      0x6000},
                                           {"clr",      0x8000},
                                           {"neg",      0x8040},
                                           {"inc",      0x8080},
                                           {"dec",      0x80c0},
                                           {"asl",      0x8100},
                                           {"asr",      0x8140},
                                           {"lsr",      0x8180},
                                           {"rol",      0x81c0},
                                           {"ror",      0x8200},
                                           {"rlc",      0x8240},
                                           {"rrc",      0x8280},
                                           {"jmp",      0x82c0},
                                           {"call",     0x8300},
                                           {"push",     0x8340},
                                           {"pop",      0x8380},
                                           {"br",       0xa000},
                                           {"bne",      0xa100},
                                           {"beq",      0xa200},
                                           {"bpl",      0xa300},
                                           {"bcs",      0xa400},
                                           {"bcc",      0xa500},
                                           {"bvs",      0xa600},
                                           {"bvc",      0xa700},
                                           {"clc",      0xc000},
                                           {"clv",      0xc001},
                                           {"clz",      0xc002},
                                           {"cls",      0xc003},
                                           {"ccc",      0xc004},
                                           {"sec",      0xc005},
                                           {"sev",      0xc006},
                                           {"sez",      0xc007},
                                           {"ses",      0xc008},
                                           {"scc",      0xc009},
                                           {"nop",      0xc00a},
                                           {"ret",      0xc00b},
                                           {"reti",     0xc00c},
                                           {"halt",     0xc00d},
                                           {"wait",     0xc00e},
                                           {"pushpc",   0xc00f},
                                           {"poppc",    0xc010},
                                           {"pushflag", 0xc011},
                                           {"popflag",  0xc012}
};

#endif //XASM_ENCODING_H
