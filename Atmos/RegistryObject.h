#pragma once

#include "Object.h"

#include "Name.h"

#include "StoredReadonlyProperty.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class RegistryObject : public Object
    {
    public:
        typedef StoredReadonlyProperty<Name> NameProperty;
        NameProperty name;
    public:
        typedef StoredReadonlyProperty<String> DescriptionProperty;
        DescriptionProperty description;
    public:
        RegistryObject(ObjectManager& manager, const Name& name);
        RegistryObject(const ::Inscription::Table<RegistryObject>& table);

        ObjectTypeDescription TypeDescription() const override;
    };

    template<>
    struct ObjectTraits<RegistryObject> : ObjectTraitsBase<RegistryObject>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::RegistryObject)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}