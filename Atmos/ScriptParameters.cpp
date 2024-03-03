#include "ScriptParameters.h"

#include "ChromaVariant.h"

#include <Inscription/VectorScribe.h>

namespace Atmos::Scripting
{
    void Parameters::Add(const Value& add)
    {
        list.push_back(add);
    }

    void Parameters::Remove(size_t index)
    {
        list.erase(begin() + index);
    }

    Parameters::Value* Parameters::Find(size_t index)
    {
        if (list.size() <= index)
            return nullptr;

        return &list[index];
    }

    const Parameters::Value* Parameters::Find(size_t index) const
    {
        if (list.size() <= index)
            return nullptr;

        return &list[index];
    }

    Parameters::SizeT Parameters::Size() const
    {
        return list.size();
    }

    bool Parameters::IsEmpty() const
    {
        return list.empty();
    }

    Parameters::iterator Parameters::begin()
    {
        return list.begin();
    }

    Parameters::const_iterator Parameters::begin() const
    {
        return list.begin();
    }

    Parameters::iterator Parameters::end()
    {
        return list.end();
    }

    Parameters::const_iterator Parameters::end() const
    {
        return list.end();
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Scripting::Parameters, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        archive(object.list);
    }
}