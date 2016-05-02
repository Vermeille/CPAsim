#pragma once

#include <string>
#include <iostream>

class WireDecl {
    std::string name_;
    int size_;

    public:
        WireDecl(const std::string& name, int num_lines = 1)
            : name_(name), size_(num_lines) {
        }

        int size() const { return size_; }
        void SetNbLines(int nb) { size_ = std::max(size_, nb); }

        const std::string& name() const { return name_; }

        void PrettyPrint() const {
            std::cout << name_ << "[" << size_ << "]";
        }
};

