#include <fstream>

#include "parser.h"

void BTT_rec(const Module& mod, ModuleValues& vals, int nb_in, int nb_bit) {
    if (nb_in == mod.input_size()) {
        mod.Execute(vals);
        for (auto i = vals.begin(); i != vals.end(); ++i) {
            std::cout << i->first << " = ";
            for (int j = 0; j < i->second->size(); ++j) {
                std::cout << i->second->At(j);
            }
            std::cout << "\t";
        }
        std::cout << "\n";
    } else {
        const WireDecl* var = mod.nth_input(nb_in);
        int next_in = (nb_bit + 1 == var->size()) ? nb_in + 1 : nb_in;
        int next_bit = (nb_bit + 1 == var->size()) ? 0 : nb_bit + 1;

        vals.ValueAt(var->name(), nb_bit) = 0;
        BTT_rec(mod, vals, next_in, next_bit);
        vals.ValueAt(var->name(), nb_bit) = 1;
        BTT_rec(mod, vals, next_in, next_bit);
    }
}

void BuildTruthTable(const Module& mod) {
    ModuleValues vals;
    BTT_rec(mod, vals, 0, 0);
}

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
