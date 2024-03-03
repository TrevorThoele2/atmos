#pragma once

#include "Object.h"
#include "ObjectReference.h"

#include "StoredReadonlyProperty.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    namespace Modulator
    {
        class ModulatedObject : public Object
        {
        public:
            typedef ObjectReference SourceReference;
            StoredReadonlyProperty<SourceReference> source;
        public:
            ModulatedObject(SourceReference source);
            ModulatedObject(const ModulatedObject& arg) = default;
            ModulatedObject(const ::Inscription::Table<ModulatedObject>& table);

            ObjectTypeDescription TypeDescription() const override;
        };
    }

    template<>
    struct ObjectTraits<Modulator::ModulatedObject> : ObjectTraitsBase<Modulator::ModulatedObject>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Modulator::ModulatedObject)
    {
    public:
        static void AddMembers(TableT& table);
    };
}