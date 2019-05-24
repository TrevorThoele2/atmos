#pragma once

#include <Chroma/Variant.h>
#include <Chroma/Iterate.h>

#include "Serialization.h"

namespace Inscription
{
    namespace detail
    {
        struct VariantSaverImplementation
        {
            template<class T>
            static void Do(T& obj, BinaryScribe& scribe)
            {
                scribe.Save(obj);
            }
        };

        template<char index>
        struct VariantLoaderImplementation
        {
            template<class... Args>
            static bool Check(::Chroma::Variant<Args...>& variant, char typeId, BinaryScribe& scribe)
            {
                typedef ::Chroma::Variant<Args...> VariantT;
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
    void Serialize(BinaryScribe& scribe, ::Chroma::Variant<Args...>& obj)
    {
        typedef ::Chroma::Variant<Args...> VariantT;
        if (scribe.IsOutput())
        {
            scribe.Save(obj.IsInhabited());
            if (!obj.IsInhabited())
                return;

            scribe.Save(static_cast<char>(obj.GetTypeAsID()));

            ::Chroma::Visit<detail::VariantSaverImplementation>(obj, scribe);
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

            ::Chroma::IterateRangeCheckStop<char, detail::VariantLoaderImplementation, bool, VariantT::count - 1, 0>(true, obj, typeAsId, scribe);
        }
    }
}