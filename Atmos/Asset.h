#pragma once

#include "FilePath.h"

#include "Serialization.h"

namespace Atmos
{
    template<class T, class Store, class Key, class Mixin>
    class AssetRegistryBase;

    class Asset
    {
    public:
        typedef unsigned int ID;
        static const ID nullID = 0;
    private:
        template<class T, class Store, class Key, class Mixin>
        friend class AssetRegistryBase;
    private:
        ID id;

        virtual String GetStringImpl() const = 0;
    public:
        Asset();
        virtual ~Asset() = 0 {}
        Asset(const Asset &arg) = default;
        Asset& operator=(const Asset &arg) = default;

        bool operator==(const Asset &arg) const;
        bool operator!=(const Asset &arg) const;

        ID GetID() const;
        String GetString() const;
    };
}