#pragma once

#include "String.h"

#include "UniqueProvider.h"

namespace Atmos
{
    class Environment
    {
    public:
        explicit Environment(String newline);

        [[nodiscard]] String Newline() const;
    private:
        String newline;
    };

    extern UniqueProvider<Environment> environment;
}