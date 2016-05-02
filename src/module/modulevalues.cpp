#include "modulevalues.h"

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
