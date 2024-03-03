#pragma once

#include <cstdint>
#include "Serialization.h"

namespace Atmos
{
    namespace Ent
    {
        class Entity
        {
        public:
            typedef std::uint32_t ValueT;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        private:
            ValueT val;
        public:
            Entity(ValueT val = 0);
            Entity(const Entity &arg) = default;
            Entity& operator=(const Entity &arg) = default;

            Entity& operator=(ValueT val);
            operator ValueT() const;
            ValueT operator*() const;
        };

        static const Entity nullEntity = 0;
    }

    using Ent::Entity;
}

namespace std
{
    template<>
    struct hash<::Atmos::Entity>
    {
        typedef ::Atmos::Entity argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type &arg) const
        {
            const result_type first(std::hash<argument_type::ValueT>()(static_cast<argument_type::ValueT>(arg)));
            return first;
        }
    };
}