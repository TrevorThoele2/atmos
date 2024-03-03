#pragma once

#include "Serialization.h"
#include <Inscription/Array.h>
#include <Function/Variant.h>
#include <Function/Iterate.h>

#include <string>

namespace inscription
{
    namespace detail
    {
        struct VariantSaverImplementation
        {
            template<class T>
            static void Do(T &obj, Scribe &scribe)
            {
                scribe.Save(obj);
            }
        };

        template<char index>
        struct VariantLoaderImplementation
        {
            template<class... Args>
            static bool Check(::function::Variant<Args...> &variant, char typeId, Scribe &scribe)
            {
                typedef ::function::Variant<Args...> VariantT;
                typedef typename VariantT::VariadicTemplateT::template Parameter<index>::Type ParameterType;

                if (index == typeId)
                {
                    ParameterType singular;
                    scribe.Load(singular);
                    variant.Set(std::move(singular));
                    return true;
                }

                return false;
            }
        };
    }

    template<class... Args>
    void Serialize(Scribe &scribe, ::function::Variant<Args...> &obj)
    {
        typedef ::function::Variant<Args...> VariantT;
        if (scribe.IsOutput())
        {
            scribe.Save(obj.IsInhabited());
            if (!obj.IsInhabited())
                return;

            scribe.Save(static_cast<char>(obj.GetTypeAsID()));

            ::function::Visit<detail::VariantSaverImplementation>(obj, scribe);
        }
        else
        {
            bool isInhabited;
            scribe.Load(isInhabited);
            if (!isInhabited)
            {
                obj.Set();
                return;
            }

            char typeAsId;
            scribe.Load(typeAsId);

            ::function::IterateRangeCheckStop<char, detail::VariantLoaderImplementation, bool, VariantT::count - 1, 0>(true, obj, typeAsId, scribe);
        }
    }
}