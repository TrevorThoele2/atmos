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

        Value* Find(size_t index);
        const Value* Find(size_t index) const;

        SizeT Size() const;
        bool IsEmpty() const;

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
    private:
        List list;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Script::Parameters, BinaryArchive> :
        public CompositeScribe<::Atmos::Script::Parameters, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}