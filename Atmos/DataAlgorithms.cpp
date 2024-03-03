#include "DataAlgorithms.h"

#include <set>

namespace Atmos
{
    void ApplyPropertyModifications(
        std::vector<Property>& properties,
        const std::vector<Property>& add,
        const std::vector<String>& remove,
        const std::vector<Property>& replace)
    {
        auto removeSet = std::set<String>(remove.begin(), remove.end());
        std::unordered_map<String, Property::Value> replaceMap;
        for (auto& property : replace)
            replaceMap.emplace(property.name, property.value);

        for (auto element = properties.begin(); element != properties.end();)
        {
            if (removeSet.find(element->name) != removeSet.end())
                element = properties.erase(element);
            else
            {
                auto foundReplace = replaceMap.find(element->name);
                if (foundReplace != replaceMap.end())
                    element->value = foundReplace->second;
                ++element;
            }
        }
        properties.insert(properties.begin(), add.begin(), add.end());
    }
}