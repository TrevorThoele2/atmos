#pragma once

#include <vector>

#include "ScriptVariant.h"

#include "Serialization.h"

namespace Atmos::Script
{
    class Parameters
    {
    public:
        using Value = Variant;
    private:
        using List = std::vector<Value>;
    public:
        using iterator = List::iterator;
        using const_iterator = List::const_iterator;

        using SizeT = List::size_type;
    public:
        void Add(const Value& add);
        void Remove(size_t index);

        [[nodiscard]] Value* Find(size_t index);
        [[nodiscard]] const Value* Find(size_t index) const;

        [[nodiscard]] SizeT Size() const;
        [[nodiscard]] bool IsEmpty() const;

        [[nodiscard]] iterator begin();
        [[nodiscard]] const_iterator begin() const;
        [[nodiscard]] iterator end();
        [[nodiscard]] const_iterator end() const;
    private:
        List list;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Script::Parameters>
    {
    public:
        using ObjectT = Atmos::Script::Parameters;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("list", object.list);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Script::Parameters, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Script::Parameters>;
    };
}