#pragma once

#include <vector>
#include <string>
#include <memory>

#include "expr.h"

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

