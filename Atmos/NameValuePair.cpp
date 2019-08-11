#include "NameValuePair.h"

#include "ChromaVariant.h"

namespace Atmos
{
    NameValuePair::NameValuePair() : name(), value()
    {}

    NameValuePair::NameValuePair(const Name& name, const VariantT& value) : name(name), value(value)
    {}

    NameValuePair::NameValuePair(Name&& name, VariantT&& value) : name(std::move(name)), value(std::move(value))
    {}

    NameValuePair::NameValuePair(const NameValuePair& arg) : name(arg.name), value(arg.value)
    {}

    NameValuePair::NameValuePair(NameValuePair&& arg) : name(std::move(arg.name)), value(std::move(arg.value))
    {}

    NameValuePair& NameValuePair::operator=(const NameValuePair& arg)
    {
        name = arg.name;
        value = arg.value;
        return *this;
    }

    NameValuePair& NameValuePair::operator=(NameValuePair&& arg)
    {
        name = std::move(arg.name);
        value = std::move(arg.value);
        return *this;
    }

    bool NameValuePair::operator==(const NameValuePair& arg) const
    {
        return name == arg.name && value == arg.value;
    }

    bool NameValuePair::operator!=(const NameValuePair& arg) const
    {
        return !(*this == arg);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::NameValuePair, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.name);
        archive(object.value);
    }

    void Scribe<::Atmos::NameValuePair, BinaryArchive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}