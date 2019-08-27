#pragma once

#include "Name.h"
#include "Variant.h"

#include "Serialization.h"

namespace Atmos
{
    class NameValuePair
    {
    public:
        Name name;
        Variant value;
    public:
        NameValuePair();
        NameValuePair(const Name& name, const Variant& value);
        NameValuePair(Name&& name, Variant&& value);
        NameValuePair(const NameValuePair& arg);
        NameValuePair(NameValuePair&& arg);

        NameValuePair& operator=(const NameValuePair& arg);
        NameValuePair& operator=(NameValuePair&& arg);

        bool operator==(const NameValuePair& arg) const;
        bool operator!=(const NameValuePair& arg) const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::NameValuePair, BinaryArchive> : public CompositeScribe<::Atmos::NameValuePair, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}