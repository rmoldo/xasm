#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "lexer.h"
#include "token.h"

int main(int argc, char **argv) {
        std::vector<std::string> types {"Instruction",
                                        "DecimalNumber",
                                        "HexNumber",
                                        "Lparan",
                                        "Rparan",
                                        "Colon",
                                        "Dot",
                                        "Comma",
                                        "Register",
                                        "Label",
                                        "NewLine",
                                        "Comment",
                                        "XASMEOF"};

        if (argc < 2) {
                std::cerr << "Please provide name of file\n";
                return EXIT_FAILURE;
        }

        std::ifstream ifs {argv[1]};

        std::string source {std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
        source += '\n';

        Lexer lexer {source};

        Token t = lexer.nextToken();

        while(t.type != TokenType::XASMEOF) {
                std::cout << types[(int)t.type] << ' ' << ((t.value == std::string{'\n'} ? "newline" : t.value)) << '\n';
                t = lexer.nextToken();
        }

        return EXIT_SUCCESS;
}
