#pragma once

#include "ShaderAsset.h"

namespace Atmos::Asset
{
    class MaterialPass
    {
    public:
        MaterialPass() = default;
        MaterialPass(Arca::Index<Shader> vertexShader, Arca::Index<Shader> fragmentShader);

        bool operator==(const MaterialPass& arg) const;
        bool operator!=(const MaterialPass& arg) const;

        [[nodiscard]] Arca::Index<Shader> VertexShader() const;
        [[nodiscard]] Arca::Index<Shader> FragmentShader() const;
    private:
        Arca::Index<Shader> vertexShader;
        Arca::Index<Shader> fragmentShader;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::MaterialPass> final
    {
    public:
        using ObjectT = Atmos::Asset::MaterialPass;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("vertexShader", object.vertexShader);
            format("fragmentShader", object.fragmentShader);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Asset::MaterialPass, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::MaterialPass>;
    };
}