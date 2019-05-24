
#include "ScriptGlobalItems.h"

#include <Inscription/Vector.h>

namespace Atmos
{
    namespace Scripting
    {
        void GlobalItems::Add(const NameValuePair& add)
        {
            list.push_back(add);
        }

        void GlobalItems::Remove(size_t index)
        {
            list.erase(begin() + index);
        }

        void GlobalItems::Remove(const Name &name)
        {
            for (auto loop = list.begin(); loop != list.end(); ++loop)
            {
                if (loop->name == name)
                {
                    list.erase(loop);
                    return;
                }
            }
        }

        NameValuePair* GlobalItems::Find(const Name &name)
        {
            for (auto& loop : list)
            {
                if (loop.name == name)
                    return &loop;
            }

            return nullptr;
        }

        const NameValuePair* GlobalItems::Find(const Name &name) const
        {
            for (auto& loop : list)
            {
                if (loop.name == name)
                    return &loop;
            }

            return nullptr;
        }

        NameValuePair* GlobalItems::Find(size_t index)
        {
            if (list.size() <= index)
                return nullptr;

            return &list[index];
        }

        const NameValuePair* GlobalItems::Find(size_t index) const
        {
            if (list.size() <= index)
                return nullptr;

            return &list[index];
        }

        NameValuePair* GlobalItems::SetOrAdd(const Name &name, const VariantT &variant)
        {
            auto found = Find(name);
            if (found)
            {
                found->value = std::move(variant);
                return found;
            }
            else
            {
                list.push_back(NameValuePair(name, variant));
                return &list.back();
            }
        }

        GlobalItems::SizeT GlobalItems::Size() const
        {
            return list.size();
        }

        bool GlobalItems::IsEmpty() const
        {
            return list.empty();
        }

        GlobalItems::iterator GlobalItems::begin()
        {
            return list.begin();
        }

        GlobalItems::const_iterator GlobalItems::begin() const
        {
            return list.begin();
        }

        GlobalItems::iterator GlobalItems::end()
        {
            return list.end();
        }

        GlobalItems::const_iterator GlobalItems::end() const
        {
            return list.end();
        }

        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DEFINE(GlobalItems)
        {
            scribe(list);
        }
    }
}