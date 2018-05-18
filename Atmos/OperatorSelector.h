#pragma once

namespace Atmos
{
    enum class OperatorSelector : unsigned char
    {
        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE
    };

    struct OperatorSelectorLessThan
    {
        bool operator()(OperatorSelector left, OperatorSelector right)
        {
            return static_cast<std::underlying_type<OperatorSelector>::type>(left) < static_cast<std::underlying_type<OperatorSelector>::type>(right);
        }
    };
}