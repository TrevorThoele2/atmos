#pragma once

#include "Sense.h"

#include "Position3D.h"
#include "StoredProperty.h"

#include "Serialization.h"

namespace Atmos
{
    class RenderFragment : public Sense
    {
    public:
        RenderFragment(ObjectManager& manager);
        RenderFragment(const RenderFragment& arg) = default;
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(RenderFragment);
        
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
    struct ObjectTraits<RenderFragment> : ObjectTraitsBase<RenderFragment>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Sense> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::RenderFragment)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}