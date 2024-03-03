#pragma once

#include "Property.h"

#include "Serialization.h"

namespace Atmos
{
    template<class Stored, class Interface = Stored>
    class StoredProperty : public Property<Interface>
    {
    public:
        StoredProperty();
        explicit StoredProperty(Stored initialValue);
        StoredProperty(const StoredProperty& arg);
        StoredProperty(StoredProperty&& arg);

        StoredProperty& operator=(const StoredProperty& arg);
        StoredProperty& operator=(Interface newValue);
    private:
        Stored stored;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };

    template<class Stored, class Interface>
    StoredProperty<Stored, Interface>::StoredProperty() :
        Property<Interface>([this]() -> Interface { return stored; }, [this](Interface newValue) { stored = newValue; }), stored()
    {}

    template<class Stored, class Interface>
    StoredProperty<Stored, Interface>::StoredProperty(Stored initialValue) :
        Property<Interface>([this]() -> Interface { return stored; }, [this](Interface newValue) { stored = newValue; }), stored(initialValue)
    {}

    template<class Stored, class Interface>
    StoredProperty<Stored, Interface>::StoredProperty(const StoredProperty& arg) :
        Property<Interface>([this]() -> Interface { return stored; }, [this](Interface newValue) { stored = newValue; }), stored(arg.stored)
    {}

    template<class Stored, class Interface>
    StoredProperty<Stored, Interface>::StoredProperty(StoredProperty&& arg) :
        Property<Interface>([this]() -> Interface { return stored; }, [this](Interface newValue) { stored = newValue; }), stored(std::move(arg.stored))
    {}

    template<class Stored, class Interface>
    StoredProperty<Stored, Interface>& StoredProperty<Stored, Interface>::operator=(const StoredProperty& arg)
    {
        this->SetInternal(arg.stored);
        return *this;
    }

    template<class Stored, class Interface>
    StoredProperty<Stored, Interface>& StoredProperty<Stored, Interface>::operator=(Interface newValue)
    {
        this->SetInternal(newValue);
        return *this;
    }

    template<class Stored, class Interface>
    void StoredProperty<Stored, Interface>::Serialize(::Inscription::Scribe& scribe)
    {
        scribe(stored);
    }
}

namespace std
{
    template<class Stored, class Interface>
    struct is_arithmetic<::Atmos::StoredProperty<Stored, Interface>> : public std::integral_constant<bool, std::is_arithmetic<Interface>::value>
    {};

    template<class Stored, class Interface>
    struct is_pointer<::Atmos::StoredProperty<Stored, Interface>> : public std::integral_constant<bool, std::is_pointer<Interface>::value>
    {};
}

namespace Atmos
{
    template<class Stored, class Interface>
    struct IsList<::Atmos::StoredProperty<Stored, Interface>> : public std::integral_constant<bool, IsList<Interface>::value>
    {};
}