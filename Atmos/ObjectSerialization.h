#pragma once

#include "ObjectStaticAssert.h"

#include "Serialization.h"

namespace Atmos
{
    template<class T>
    class ObjectInscripter : public ::Inscription::InscripterBase<T>
    {
    private:
        typedef ObjectTraits<T> TraitsT;
        template<class U>
        using HasBaseList = typename TraitsT::template HasBaseList<U>;

        typedef ::Inscription::InscripterBase<T> BaseT;
    public:
        typedef typename BaseT::TableT TableT;
    public:
        static TableT CreateTable(::Inscription::Scribe& scribe)
        {
            TableT table;

            AttemptAddBases<T>(table);

            ::Inscription::Inscripter<T>::AddMembers(table);

            return table;
        }

        static ::Inscription::ClassNameResolver classNameResolver;
    private:
        template<::Chroma::VariadicTemplateSize i>
        struct BaseIterator
        {
            template<class... BaseTypes>
            static void Do(const ObjectTypeList<BaseTypes...>& baseList, TableT& table)
            {
                typedef ObjectTypeList<BaseTypes...> BaseListT;
                typedef typename BaseListT::template Parameter<i>::Type PieceType;

                table.AddBase<PieceType>();
            }
        };

        template<class U, typename std::enable_if<HasBaseList<U>::value, int>::type = 0>
        static void AttemptAddBases(TableT& table)
        {
            TraitsT::template IterateOverBases<BaseIterator>(TraitsT::bases, table);
        }

        template<class U, typename std::enable_if<!HasBaseList<U>::value, int>::type = 0>
        static void AttemptAddBases(TableT& table)
        {
            static constexpr ObjectTypeList<Object> defaultBases = {};
            TraitsT::template IterateOverBases<BaseIterator>(defaultBases, table);
        }
    private:
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT(T);
    };

    template<class T>
    ::Inscription::ClassNameResolver ObjectInscripter<T>::classNameResolver([](::Inscription::Scribe& scribe)
    {
        return TypeNameFor<T>();
    });
}

#define DECLARE_OBJECT_INSCRIPTER(T)                        \
template<>                                                  \
class Inscripter<T> : public ::Atmos::ObjectInscripter<T>

#define DEFINE_OBJECT_INSCRIPTER_MEMBERS(T)     \
void Inscripter<T>::AddMembers(TableT& table)