#pragma once

#include <unordered_map>
#include "String.h"

namespace Atmos
{
    class InitializationFileExtractedSection
    {
    public:
        String header;

        typedef std::unordered_map<String, String> NameToValue;
        NameToValue nameValues;

        bool IsOccupied() const;
    };
}