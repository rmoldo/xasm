//
// Created by moldo on 13/03/2021.
//

#ifndef XASM_DEFS_H
#define XASM_DEFS_H

#include <vector>
#include <string>
#include <map>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef std::map<std::string, u16> Labels;


static std::vector<std::string> instructionVector {"mov", "add", "sub", "cmp", "and", "or", "xor", "clr", "neg", "inc", "dec",
                                                   "asl", "asr", "lsr", "rol", "ror", "rlc", "rrc", "jmp", "call", "push", "pop",
                                                   "br", "bne", "beq", "bpl", "bcs", "bcc", "bvs", "bvc", "clc", "clv", "clz",
                                                   "cls", "ccc", "sec", "sev", "sez", "ses", "scc", "nop", "ret", "reti","halt", "wait",
                                                   "pushflag", "popflag", "pushpc", "poppc"};


#endif //XASM_DEFS_H
