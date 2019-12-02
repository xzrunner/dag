#pragma once

#include "hdiop/VarType.h"

#include <SM_Vector.h>

#include <string>

namespace hdiop
{

class Variable
{
public:
    Variable() : type(VarType::Invalid) {}
    explicit Variable(bool b);
    explicit Variable(int  i);
    explicit Variable(float f);
    explicit Variable(double d);
    explicit Variable(const std::string& str);
    explicit Variable(const sm::vec2& v2);
    explicit Variable(const sm::vec3& v3);
    explicit Variable(const sm::vec4& v4);
    Variable(const Variable& var);
    Variable& operator = (const Variable& var);
    ~Variable();

    bool operator == (const Variable& var) const;

    VarType  type = VarType::Invalid;
    union
    {
        bool   b;
        int    i;
        float  f;
        double d;
        const void* p = nullptr;
    };

}; // Variable

}