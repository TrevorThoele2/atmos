#pragma once

namespace Atmos
{
    template<class CheckType, class InputType>
    bool IsSameRuntimeType(const InputType& inputType)
    {
        auto casted = dynamic_cast<const CheckType*>(&inputType);
        return casted != nullptr;
    }

    template<class CheckType, class InputType>
    bool IsSameRuntimeType(const InputType* const inputType)
    {
        auto casted = dynamic_cast<const CheckType* const>(inputType);
        return casted != nullptr;
    }
}