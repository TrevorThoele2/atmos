#pragma once

#include "Serialization.h"

namespace Atmos::Render
{
    class Color
    {
    public:
        using Value = unsigned char;

        Value alpha;
        Value red;
        Value green;
        Value blue;
    public:
        Color();
        Color(Value alpha, Value red, Value green, Value blue);
        Color(const Color& arg);

        Color& operator=(const Color& arg);
        bool operator==(const Color& arg) const;
        bool operator!=(const Color& arg) const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::Color, BinaryArchive> :
        public CompositeScribe<::Atmos::Render::Color, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}