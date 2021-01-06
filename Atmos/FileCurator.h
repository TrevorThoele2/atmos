#pragma once

#include <Arca/Curator.h>
#include "ExtractFile.h"

namespace Atmos::File
{
    class Curator : public Arca::Curator
    {
    public:
        Curator(Init init);
    public:
        Buffer Handle(const ExtractFile& command);
        Buffer Handle(const ExtractZippedFile& command);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::File::Curator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::File::Curator"; }
        using HandledCommands = HandledCommands<Atmos::File::ExtractFile, Atmos::File::ExtractZippedFile>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::File::Curator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::File::Curator>;
    };
}