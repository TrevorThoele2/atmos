#include "DataCore.h"

namespace Atmos
{
    DataCore::DataCore(const std::vector<Property>& properties) : properties(properties)
    {}

    bool DataCore::operator==(const DataCore& arg) const
    {
        return properties == arg.properties;
    }

    bool DataCore::operator!=(const DataCore& arg) const
    {
        return !(*this == arg);
    }
}