#include "modulevalues.h"

#include <iostream>

int& ModuleValues::ValueAt(const std::string& name, int bit) {
    auto val = vals_.find(name);
    if (val != vals_.end()) {
        return val->second->At(bit);
    } else {
        auto new_val = vals_.emplace(
                std::make_pair(name,
                    std::unique_ptr<WireValue>(new WireValue))).first;
        return new_val->second->At(bit);
    }
}

void ModuleValues::Print() const {
    for (auto i = vals_.begin(); i != vals_.end(); ++i) {
        std::cout << i->first << " = ";
        for (int j = 0; j < i->second->size(); ++j) {
            std::cout << i->second->At(j);
        }
        std::cout << "\t";
    }
    std::cout << "\n";
}
