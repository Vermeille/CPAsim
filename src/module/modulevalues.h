#pragma once

#include <map>
#include <memory>
#include "../wire/wirevalue.h"

class ModuleValues {
    std::map<std::string, std::unique_ptr<WireValue>> vals_;
    public:
        int& ValueAt(const std::string& name, int bit);

        int ValueAt(const std::string& name, int bit) const {
            return vals_.at(name)->At(bit);
        }

        std::map<std::string, std::unique_ptr<WireValue>>::iterator begin() {
            return vals_.begin();
        }

        std::map<std::string, std::unique_ptr<WireValue>>::iterator end() {
            return vals_.end();
        }
};

