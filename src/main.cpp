#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "XASMGenerator.h"

int main(int argc, char **argv) {
        if (argc < 2) {
                std::cerr << "Please provide name of file\n";
                return EXIT_FAILURE;
        }

        std::ifstream ifs{argv[1]};

        std::string source{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
        source += '\n';

        Lexer lexer{source};
        XASMParser parser{lexer};
        Labels labels;

        try {
                parser.parse();
                labels = parser.getLabels();
        } catch (std::exception &e) {
                std::cerr << "Error during parsing: " << e.what();
                return EXIT_FAILURE;
        }

        std::cout << "Parsed successfuly\n";

        lexer.rewind();
        XASMGenerator generator{lexer, labels};

        try {
                generator.generate();
                std::cout << "Binary generated successfully\n";
        } catch (std::exception &e) {
                std::cerr << "Error during generating: " << e.what();
                return EXIT_FAILURE;
        }
        
        return EXIT_SUCCESS;
}
