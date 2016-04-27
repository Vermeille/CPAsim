#include <fstream>

#include "parser.h"

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "usage: " << argv[0] << " <module>\n";
        return 1;
    }

    std::ifstream input_file(argv[1]);
    if (!input_file) {
        std::cerr << "can't read input file\n";
        return 1;
    }

    Parser::ParseModuleDef(input_file)->PrettyPrint();
    return 0;
}
