#pragma once

#include <unordered_map>
#include "String.h"

namespace Atmos::Initialization
{
    class FileExtractedSection
    {
    public:
        String header;

        using NameToValue = std::unordered_map<String, String>;
        NameToValue nameValues;

        bool IsOccupied() const;
    };
}