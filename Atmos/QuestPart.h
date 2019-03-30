#pragma once

#include "Name.h"

#include "Serialization.h"

namespace Atmos
{
    class QuestPart
    {
    public:
        typedef String Description;

        enum class Type
        {
            WAR,
            DIPLOMACY,
            GATHER,
            EXPLORE,
            MISC
        };
    public:
        Name name;
        Description description;
        Type type;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}