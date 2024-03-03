#pragma once

#include <vector>
#include <unordered_set>
#include "Property.h"

namespace Atmos
{
    namespace Detail
    {
        template<class T, class Itr>
        void Insert(std::vector<T>& modify, Itr begin, Itr end)
        {
            modify.insert(modify.begin(), begin, end);
        }

        template<class T, class Itr>
        void Insert(std::unordered_set<T>& modify, Itr begin, Itr end)
        {
            modify.insert(begin, end);
        }
    }

    template<class Modify, class T>
    void ApplyAddRemoveModifications(
        Modify& modify,
        const std::vector<T>& add,
        const std::vector<T>& remove)
    {
        auto removeSet = std::unordered_set<T>(remove.begin(), remove.end());

        for (auto element = modify.begin(); element != modify.end();)
        {
            if (removeSet.find(*element) != removeSet.end())
                element = modify.erase(element);
            else
                ++element;
        }
        Detail::Insert(modify, add.begin(), add.end());
    }

    void ApplyPropertyModifications(
        std::vector<Property>& properties,
        const std::vector<Property>& add,
        const std::vector<String>& remove,
        const std::vector<Property>& replace);
}