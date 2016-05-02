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

        void Overwrite(ModuleValues&& new_vals) {
            for (auto& v : new_vals.vals_) {
                vals_[v.first] = std::move(v.second);
            }
            new_vals.vals_.clear();
        }

        std::map<std::string, std::unique_ptr<WireValue>>::iterator begin() {
            return vals_.begin();
        }

        std::map<std::string, std::unique_ptr<WireValue>>::iterator end() {
            return vals_.end();
        }

        void Print() const;
};

