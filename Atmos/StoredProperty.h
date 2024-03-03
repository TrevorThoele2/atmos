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
        stored = arg;
        this->SetInternal(stored);
        return *this;
    }

    template<class Stored, class Interface>
    StoredProperty<Stored, Interface>& StoredProperty<Stored, Interface>::operator=(Interface newValue)
    {
        stored = newValue;
        this->SetInternal(stored);
        return *this;
    }
}

namespace Inscription
{
    template<class Stored, class Interface>
    class Scribe<::Atmos::StoredProperty<Stored, Interface>, BinaryArchive> :
        public CompositeScribe<::Atmos::StoredProperty<Stored, Interface>, BinaryArchive>
    {
    private:
        using BaseT = typename CompositeScribe<::Atmos::StoredProperty<Stored, Interface>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive)
        {
            archive(object.stored);
        }
    };
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