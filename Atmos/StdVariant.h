#pragma once

#include <variant>
#include <Chroma/Iterate.h>

#include "Serialization.h"

namespace Inscription
{
    namespace detail
    {
        struct VariantSaverImplementation
        {
            OutputBinaryArchive* archive;

            template<class T>
            void operator()(T& object)
            {
                (*archive)(object);
            }
        };

        template<char index>
        struct VariantLoaderImplementation
        {
            template<class... Args>
            static bool Check(std::variant<Args...>& variant, char typeId, InputBinaryArchive& archive)
            {
                using ParameterType = typename Chroma::VariadicTemplate<Args...>::template Parameter<index>::Type;

                if (index == typeId)
                {
                    ParameterType singular;
                    archive(singular);
                    variant = std::move(singular);
                    return true;
                }

                return false;
            }
        };
    }

    template<class... Args>
    class Scribe<std::variant<Args...>, BinaryArchive> :
        public CompositeScribe<std::variant<Args...>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<std::variant<Args...>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;

        using BaseT::Scriven;
    protected:
        void ScrivenImplementation(ObjectT& object, BinaryArchive& archive) override
        {
            if (archive.IsOutput())
            {
                size_t index = object.index();
                archive(index);

                std::visit(detail::VariantSaverImplementation{ archive.AsOutput() }, object);
            }
            else
            {
                char typeAsId;
                archive(typeAsId);

                ::Chroma::IterateRangeCheckStop<
                    char,
                    detail::VariantLoaderImplementation,
                    bool, sizeof...(Args) - 1,
                    0
                >(true, object, typeAsId, *archive.AsInput());
            }
        }
    };
}