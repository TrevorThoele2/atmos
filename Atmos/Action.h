#pragma once

#include "String.h"
#include "Input.h"

namespace Atmos::Input
{
    struct Action
    {
        const String displayName;

        Input* mappedInput = nullptr;

        explicit Action(String displayName);
    };
}
