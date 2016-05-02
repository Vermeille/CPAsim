#pragma once

#include <vector>

class WireValue {
    std::vector<int> values_;
    public:
        size_t size() const { return values_.size(); }
        int At(int idx) const {
            return values_[idx];
        }

        int& At(int idx) {
            if (idx >= values_.size()) {
                values_.resize(idx + 1, 0);
            }
            return values_[idx];
        }
};

