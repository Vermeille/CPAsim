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

    std::unique_ptr<Module> mod(Parser::ParseModuleDef(input_file));
    mod->BindUsagesToDef();
    ModuleValues vals;

    vals.ValueAt("a", 0) = 0;
    vals.ValueAt("b", 0) = 0;
    mod->Execute(vals);
    std::cout << vals.ValueAt("out", 0) << std::endl;

    vals.ValueAt("a", 0) = 1;
    vals.ValueAt("b", 0) = 0;
    mod->Execute(vals);
    std::cout << vals.ValueAt("out", 0) << std::endl;

    vals.ValueAt("a", 0) = 0;
    vals.ValueAt("b", 0) = 1;
    mod->Execute(vals);
    std::cout << vals.ValueAt("out", 0) << std::endl;

    vals.ValueAt("a", 0) = 1;
    vals.ValueAt("b", 0) = 1;
    mod->Execute(vals);
    std::cout << vals.ValueAt("out", 0) << std::endl;
    return 0;
}
