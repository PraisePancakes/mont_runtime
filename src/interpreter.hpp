#pragma once
#include <iostream>
#include "frontend/interfaces/visitor.hpp"

// propogate error state up to Interpreter
// Interpreter dispatches error state

namespace MPROCESS
{
    class Interpreter //: public IExprVisitor<void *>
    {

    public:
        Interpreter();

        ~Interpreter();
    };
};