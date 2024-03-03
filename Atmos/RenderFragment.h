#pragma once

#include "Sense.h"

#include "Position3D.h"
#include "StoredProperty.h"

#include "Serialization.h"

namespace Atmos
{
    class nRenderFragment : public nSense
    {
    public:
        nRenderFragment();
        nRenderFragment(const nRenderFragment& arg) = default;
        nRenderFragment(const ::Inscription::Table<nRenderFragment>& table);
        
        void Draw();

        ObjectTypeDescription TypeDescription() const override;
    private:
        virtual void DrawImpl() = 0;
    private:
        void SubscribeToProperties();
    private:
        void OnEnabledChanged(bool newValue);
    };

    template<>
    struct ObjectTraits<nRenderFragment> : ObjectTraitsBase<nRenderFragment>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<nSense> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::nRenderFragment)
    {
    public:
        static void AddMembers(TableT& table);
    };
}