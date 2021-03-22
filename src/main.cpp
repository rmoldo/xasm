#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "lexer.h"
#include "token.h"
#include "parser.h"

int main(int argc, char **argv) {
        std::vector<std::string> types {"Instruction",
                                        "Number",
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
        XASMParser parser {lexer};

        parser.parse();

        /*
        Token t = lexer.nextToken();

        while(t.type != TokenType::XASMEOF) {
                std::cout << types[(int)t.type] << ' ' << ((t.value == std::string{'\n'} ? "newline" : t.value)) << '\n';
                t = lexer.nextToken();
        }

         */

        std::cout << "Parsed successfuly\n";
        return EXIT_SUCCESS;
}
