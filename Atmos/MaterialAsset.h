#pragma once

#include "Asset.h"
#include <Arca/ClosedTypedRelicAutomation.h>

#include "ImageAsset.h"
#include "ShaderAsset.h"

namespace Atmos::Asset
{
    class MaterialAsset final : public Arca::ClosedTypedRelicAutomation<MaterialAsset>, public Asset
    {
    public:
        using GridDimension = int;
        [[nodiscard]] GridDimension Columns() const;
        [[nodiscard]] GridDimension Rows() const;
    public:
        [[nodiscard]] ImageAsset* Image() const;
        [[nodiscard]] ShaderAsset* Shader() const;
    public:
        MaterialAsset();
        MaterialAsset(const MaterialAsset& arg) = delete;
        MaterialAsset(MaterialAsset&& arg) noexcept = default;
        explicit MaterialAsset(const ::Inscription::BinaryTableData<MaterialAsset>& data);
    public:
        void Initialize(const Atmos::Name& name);
    private:
        GridDimension columns = 0;
        GridDimension rows = 0;

        ImageAsset* image = nullptr;
        ShaderAsset* shader = nullptr;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Asset::MaterialAsset>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
        static bool ShouldCreate(Reliquary& reliquary, const ::Atmos::Name& name);
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Asset::MaterialAsset, BinaryArchive> :
        TableDataBase<::Atmos::Asset::MaterialAsset, BinaryArchive>
    {
        Base<::Atmos::Asset::Asset> base;
    };

    template<>
    class Scribe<::Atmos::Asset::MaterialAsset, BinaryArchive> final :
        public ArcaTableScribe<::Atmos::Asset::MaterialAsset, BinaryArchive>
    {
    public:
        class Table final : public TableBase
        {
        public:
            Table();
        };
    };
}