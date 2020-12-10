#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Bits.h
Purpose:		Utility class for handling bit vectors. Derived from:
                https://faculty.digipen.edu/~dvolper/public/cs225/bitvector.html
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Nov-27
*************************************************************************/
#include <fstream>
#include <iostream>

namespace VannoEngine {
    template< int N  >
    class Bits {
        int data;
        enum { multiplyer = ((N > 8 * sizeof(int)) ? 1 : 0) };
        Bits< (N - sizeof(int) * 8)* multiplyer > rest;
    public:
        Bits() : data(0) { }
        ////////////////////////////////////////////////////////////////////////
        void Set(unsigned index) {
            if (index < 8 * sizeof(int)) { data |= 1 << index; }
            else { rest.Set(index - 8 * sizeof(int)); }
        }
        ////////////////////////////////////////////////////////////////////////
        void Clear(unsigned index) {
            if (index < 8 * sizeof(int)) { data &= ~(1 << index); }
            else { rest.Clear(index - 8 * sizeof(int)); }
        }
        ////////////////////////////////////////////////////////////////////////
        bool Get(unsigned index) const {
            if (index < 8 * sizeof(int)) { return data & (1 << index); }
            return rest.Get(index - 8 * sizeof(int));
        }
        ////////////////////////////////////////////////////////////////////////
        void Toggle(unsigned index) {
            if (index < 8 * sizeof(int)) { data ^= ~(1 << index); }
            else { rest.Toggle(index - 8 * sizeof(int)); }
        }
        ////////////////////////////////////////////////////////////////////////
        void Reset() {
            data = 0;
            rest.Reset();
        }
        ////////////////////////////////////////////////////////////////////////
        Bits& operator^=(Bits const& op2) {
            data ^= op2.data;
            rest ^= op2.rest;
            return *this;
        }
        ////////////////////////////////////////////////////////////////////////
        Bits& operator&=(Bits const& op2) {
            data &= op2.data;
            rest &= op2.rest;
            return *this;
        }
        ////////////////////////////////////////////////////////////////////////
        Bits& operator|=(Bits const& op2) {
            data |= op2.data;
            rest |= op2.rest;
            return *this;
        }
        ////////////////////////////////////////////////////////////////////////
        Bits const operator~() const {
            Bits result(*this);
            result.data = ~result.data;
            result.rest = ~result.rest;
            return result;
        }
        ////////////////////////////////////////////////////////////////////////
        Bits const operator^(Bits const& op2) const { return Bits(*this) ^= op2; }
        Bits const operator&(Bits const& op2) const { return Bits(*this) &= op2; }
        Bits const operator|(Bits const& op2) const { return Bits(*this) |= op2; }
        ////////////////////////////////////////////////////////////////////////
        friend std::ostream& operator<<(std::ostream& os, Bits const& b) {
            for (unsigned i = 0; i < 8 * sizeof(int); ++i) { os << b.Get(i); }
            os << "-" << b.rest;
            return os;
        }
    };

    template<>
    class Bits<0> {
    public:
        void Set(unsigned index) {}
        bool Get(unsigned index) const { return false; }
        void Clear(unsigned index) { }
        void Toggle(unsigned index) { }
        void Reset() { }
        Bits& operator|=(Bits const& op2) { return *this; }
        Bits& operator&=(Bits const& op2) { return *this; }
        Bits& operator^=(Bits const& op2) { return *this; }
        Bits const operator~()                 const { return Bits<0>(); }
        Bits const operator|(Bits const& op2) const { return Bits<0>(); }
        Bits const operator&(Bits const& op2) const { return Bits<0>(); }
        Bits const operator^(Bits const& op2) const { return Bits<0>(); }
        friend std::ostream& operator<<(std::ostream& os, Bits const& b) { return os; }
    };
}