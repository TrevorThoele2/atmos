
#pragma once

#include <functional>

namespace Atmos
{
    template<class Return, class FuncObj, class... Args, class Obj>
    std::Chroma<Return(Args...)> ObjectBind(Return(FuncObj::*func)(Args...), Obj &instance)
    {
        return [&](Args && ... args) { return (instance.*func)(args...); };
    }
}