#pragma once

#include <vector>
#include <string>
#include <memory>

#include "expr.h"

class WireDecl {
    std::string name_;
    std::vector<int> values_;

    public:
        WireDecl(const std::string& name, int num_lines = 1)
            : name_(name), values_(num_lines, 0) {
        }

        int size() const { return values_.size(); }
        int At(int idx) { return values_[idx]; }
        void SetNbLines(int nb) { values_.resize(nb, 0); }

        const std::string& name() const { return name_; }

        void PrettyPrint() const {
            std::cout << name_ << "[" << values_.size() << "]";
        }
};

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

        virtual int Exec() {
            return decl_->At(index_);
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

class OutputDef {
    std::unique_ptr<WireUsage> dst_;
    std::unique_ptr<Expr> src_;

    public:
        OutputDef(WireUsage* wire, Expr* expr)
            : dst_(wire), src_(expr) {
        }

        const std::string& name() const { return dst_->name(); }
        int index() const { return dst_->index(); }

        void PrettyPrint() const {
            dst_->PrettyPrint();
            std::cout << " = ";
            src_->PrettyPrint();
        }

        Expr* expr() const { return src_.get(); }
};

