#pragma once

#include <string>
#include <iostream>

#include "wiredecl.h"

class WireUsage : public Expr {
    std::string name_;
    int index_;
    WireDecl* decl_;

    public:
        WireUsage(const std::string& name, int idx = 0)
            : name_(name), index_(idx), decl_(nullptr) {
        }

        void PrettyPrint() const {
            std::cout << name_ << "[" << index_ << "]";
        }

        virtual int Exec(ModuleValues& val) {
            return val.ValueAt(name_, index_);
        }

        const std::string& name() const { return name_; }
        int index() const { return index_; }

        void SetDeclRef(WireDecl* decl) {
            decl_ = decl;
        }

        bool IsUseValid() const {
            return decl_ && index_ < decl_->size();
        }
};

