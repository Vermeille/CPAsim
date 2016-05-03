#pragma once

#include <memory>
#include <iostream>

#include "module/modulevalues.h"

class Expr {
    public:
        virtual int Exec(ModuleValues& val) = 0;
        virtual void PrettyPrint() const = 0;
};

class Binop : public Expr {
    std::unique_ptr<Expr> lhs_;
    std::unique_ptr<Expr> rhs_;
    public:
        Binop(std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs)
            : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {
        }

        Expr* lhs() const { return lhs_.get(); }
        Expr* rhs() const { return rhs_.get(); }
};

class Not : public Expr {
    std::unique_ptr<Expr> rhs_;
    public:
        virtual int Exec(ModuleValues& val) {
            return !rhs_->Exec(val);
        }

        Not(std::unique_ptr<Expr> rhs)
            : rhs_(std::move(rhs)) {
        }

        Expr* rhs() const { return rhs_.get(); }

        virtual void PrettyPrint() const {
            std::cout << "/(";
            rhs_->PrettyPrint();
            std::cout << ")";
        }
};

class Or : public Binop {
    public:
        virtual int Exec(ModuleValues& val) {
            return lhs()->Exec(val) | rhs()->Exec(val);
        }

        Or(std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs)
            : Binop(std::move(lhs), std::move(rhs)) {
        }

        virtual void PrettyPrint() const {
            std::cout << "(";
            lhs()->PrettyPrint();
            std::cout << ") + (";
            rhs()->PrettyPrint();
            std::cout << ")";
        }
};

class And : public Binop {
    public:
        virtual int Exec(ModuleValues& val) {
            return lhs()->Exec(val) & rhs()->Exec(val);
        }

        And(std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs)
            : Binop(std::move(lhs), std::move(rhs)) {
        }

        virtual void PrettyPrint() const {
            std::cout << "(";
            lhs()->PrettyPrint();
            std::cout << ") . (";
            rhs()->PrettyPrint();
            std::cout << ")";
        }
};

class Xor : public Binop {
    public:
        virtual int Exec(ModuleValues& val) {
            return lhs()->Exec(val) ^ rhs()->Exec(val);
        }

        Xor(std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs)
            : Binop(std::move(lhs), std::move(rhs)) {
        }

        virtual void PrettyPrint() const {
            std::cout << "(";
            lhs()->PrettyPrint();
            std::cout << ") ^ (";
            rhs()->PrettyPrint();
            std::cout << ")";
        }
};

class Constant : public Expr {
    int value_;
    public:
        virtual int Exec(ModuleValues&) {
            return value_;
        }

        Constant(int val) : value_(val) {}

        virtual void PrettyPrint() const {
            std::cout << value_;
        }
};

