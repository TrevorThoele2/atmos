#pragma once

#include <string>

#include <Inscription/BinaryFormat.h>
#include <Inscription/JsonFormat.h>
#include <Inscription/Version.h>
#include <Inscription/CompositeScribeCategory.h>

namespace Atmos
{
    struct FileHeader
    {
        std::string signature;
        std::string expectedSignature;
        Inscription::Version inscriptionVersion;
        Inscription::Version clientVersion;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::FileHeader> final
    {
    public:
        using ObjectT = Atmos::FileHeader;
    public:
        void Scriven(ObjectT& object, Inscription::Format::Binary& format);
        void Scriven(ObjectT& object, Inscription::Format::Json& format);
    };

    template<class Format>
    struct ScribeTraits<Atmos::FileHeader, Format> final
    {
        using Category = CompositeScribeCategory<Atmos::FileHeader>;
    };
}