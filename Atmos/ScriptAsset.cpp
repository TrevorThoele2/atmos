#include "ScriptAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    Script::Script(Init init) : FileAsset(init)
    {}

    Script::Script(Init init, const ::Atmos::Name& name, DataPtr&& data) :
        FileAsset(init, name, std::move(data))
    {}

    Script::Script(Script&& arg) noexcept : FileAsset(std::move(arg))
    {}

    Script& Script::operator=(Script&& arg) noexcept
    {
        FileAsset::operator=(std::move(arg));
        return *this;
    }
}

namespace Arca
{
    bool Traits<::Atmos::Asset::Script>::ShouldCreate(
        Reliquary& reliquary, const ::Atmos::Name& name, const ::Atmos::Asset::Script::DataPtr& data)
    {
        return Atmos::Asset::ShouldCreate<::Atmos::Asset::Script>(reliquary, name);
    }
}

namespace Inscription
{
    void Scribe<Atmos::Asset::Script, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<Atmos::Asset::FileAsset<Atmos::Asset::ScriptData, Atmos::Asset::Script>>(
            object, archive);
    }
}