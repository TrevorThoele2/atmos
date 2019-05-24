#pragma once

#include <vector>

#include "NameValuePair.h"

#include "Serialization.h"

namespace Atmos
{
    namespace Scripting
    {
        class GlobalItems
        {
        private:
            typedef std::vector<NameValuePair> List;
        public:
            typedef List::iterator iterator;
            typedef List::const_iterator const_iterator;

            typedef List::size_type SizeT;
        public:
            void Add(const NameValuePair& add);

            void Remove(size_t index);

            void Remove(const Name& name);
            NameValuePair* Find(const Name& name);
            const NameValuePair* Find(const Name& name) const;
            NameValuePair* Find(size_t index);
            const NameValuePair* Find(size_t index) const;

            // If the global item exists, then sets it. Otherwise adds it and sets it to the variant
            NameValuePair* SetOrAdd(const Name& name, const VariantT& variant);

            SizeT Size() const;
            bool IsEmpty() const;

            iterator begin();
            const_iterator begin() const;
            iterator end();
            const_iterator end() const;
        private:
            typedef std::vector<NameValuePair> List;
            List list;
        private:
            INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        };
    }
}