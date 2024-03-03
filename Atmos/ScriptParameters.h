#pragma once

#include <vector>

#include "ScriptVariant.h"

#include "Serialization.h"

namespace Atmos
{
    namespace Scripting
    {
        class Parameters
        {
        public:
            typedef VariantT Value;
        private:
            typedef std::vector<Value> List;
        public:
            typedef List::iterator iterator;
            typedef List::const_iterator const_iterator;

            typedef List::size_type SizeT;
        public:
            void Add(const Value& add);
            void Remove(size_t index);

            Value* Find(size_t index);
            const Value* Find(size_t index) const;

            SizeT Size() const;
            bool IsEmpty() const;

            iterator begin();
            const_iterator begin() const;
            iterator end();
            const_iterator end() const;
        private:
            List list;
        private:
            INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        };
    }
}