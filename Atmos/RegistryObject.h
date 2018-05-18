#pragma once

#include "Name.h"
#include "String.h"

#include "Serialization.h"

namespace Atmos
{
    class RegistryObject
    {
    public:
        typedef String Description;
        typedef unsigned int ID;
        static const ID nullID = 0;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        template<class T, class Mixin>
        friend class RegistryBase;
    protected:
        Name name;
        ID id;
    public:
        Name niceName;
        Description description;

        RegistryObject() = default;
        RegistryObject(const RegistryObject &arg) = default;
        RegistryObject(RegistryObject &&arg);
        RegistryObject& operator=(const RegistryObject &arg) = default;
        RegistryObject& operator=(RegistryObject &&arg);
        virtual ~RegistryObject() = 0 {}

        bool operator==(const RegistryObject &arg) const;
        bool operator!=(const RegistryObject &arg) const;

        const Name& GetName() const;
        ID GetID() const;
        const Name& GetNiceName() const;
        const Description& GetDescription() const;
    };
}