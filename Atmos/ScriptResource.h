#pragma once

#include "ScriptResult.h"

namespace Atmos::Scripting
{
    class Resource
    {
    public:
        virtual ~Resource() = 0;

        virtual std::optional<Result> Execute() = 0;

        virtual std::optional<Result> Resume() = 0;
        virtual void Suspend() = 0;
    };
}