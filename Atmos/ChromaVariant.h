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
            static void Do(T& object, OutputBinaryArchive& archive)
            {
                archive(object);
            }
        };

        template<char index>
        struct VariantLoaderImplementation
        {
            template<class... Args>
            static bool Check(::Chroma::Variant<Args...>& variant, char typeId, InputBinaryArchive& archive)
            {
                typedef ::Chroma::Variant<Args...> VariantT;
                typedef typename VariantT::VariadicTemplateT::template Parameter<index>::Type ParameterType;

                if (index == typeId)
                {
                    ParameterType singular;
                    archive(singular);
                    variant.Set(std::move(singular));
                    return true;
                }

                return false;
            }
        };
    }

    template<class... Args>
    class Scribe<::Chroma::Variant<Args...>, BinaryArchive> : public CompositeScribe<::Chroma::Variant<Args...>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<::Chroma::Variant<Args...>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        static void Scriven(ObjectT& object, BinaryArchive& archive)
        {
            typedef ::Chroma::Variant<Args...> VariantT;
            if (archive.IsOutput())
            {
                auto inhabited = object.IsInhabited();
                archive(inhabited);
                if (!object.IsInhabited())
                    return;

                auto type = static_cast<char>(object.GetTypeAsID());
                archive(type);

                ::Chroma::Visit<detail::VariantSaverImplementation>(object, *archive.AsOutput());
            }
            else
            {
                bool isInhabited;
                archive(isInhabited);
                if (!isInhabited)
                {
                    object.Set();
                    return;
                }

                char typeAsId;
                archive(typeAsId);

                ::Chroma::IterateRangeCheckStop<
                    char,
                    detail::VariantLoaderImplementation,
                    bool, VariantT::count - 1,
                    0
                >(true, object, typeAsId, *archive.AsInput());
            }
        }
    };
}