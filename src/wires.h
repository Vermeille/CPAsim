#pragma once

#include <vector>
#include <string>

class WireDecl {
    std::string name_;
    std::vector<int> values_;

    public:
        WireDecl(const std::string& name, int num_lines)
            : name_(name), values_(num_lines) {
        }

        WireDecl(const std::string& name)
            : name_(name), values_(1) {
        }

        void PrettyPrint() const {
            std::cout << name_ << "[" << values_.size() << "]";
        }
};

