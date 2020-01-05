#include "Environment.h"

namespace Atmos
{
    Environment::Environment(String newline) : newline(std::move(newline))
    {}

    String Environment::Newline() const
    {
        return newline;
    }

    UniqueProvider<Environment> environment;
}