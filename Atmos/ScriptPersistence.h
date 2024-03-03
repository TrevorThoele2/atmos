#pragma once

#include "Name.h"

#include "Serialization.h"

namespace Atmos
{
    namespace Scripting
    {
        class Persistence
        {
        public:
            typedef size_t SizeT;
        private:
            class Impl;
        public:
            Persistence();
            Persistence(const Persistence& arg);
            ~Persistence();

            Persistence& operator=(const Persistence& arg);

            SizeT Size() const;
            bool IsEmpty() const;
        private:
            INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        };
    }
}