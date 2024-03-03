#pragma once

#include "Name.h"

#include "Serialization.h"

namespace Atmos::Script
{
    class Persistence
    {
    public:
        using SizeT = size_t;
    private:
        class Impl;
    public:
        Persistence();
        Persistence(const Persistence& arg);
        ~Persistence();

        Persistence& operator=(const Persistence& arg);

        SizeT Size() const;
        bool IsEmpty() const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Script::Persistence> final
    {
    public:
        using ObjectT = Atmos::Script::Persistence;
    public:
        template<class Archive>
        void Scriven(ObjectT&, Archive&)
        {}
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Script::Persistence, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Script::Persistence>;
    };
}