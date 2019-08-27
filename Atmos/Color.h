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
        Color(const Color& arg) = default;

        Color& operator=(const Color& arg) = default;

        bool operator==(const Color& arg) const;
        bool operator!=(const Color& arg) const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::Color, BinaryArchive> final :
        public CompositeScribe<::Atmos::Render::Color, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}