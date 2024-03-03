#pragma once

#include "nModulatorLink.h"

namespace Atmos
{
    namespace Modulator
    {
        template<class ClassT, class LinkT>
        class LinkGenerator
        {
        public:
            typedef Link::Getter<ClassT> Getter;
            typedef Link::Setter<ClassT, LinkT> Setter;
        public:
            const LinkID id;
        public:
            LinkGenerator(LinkID id, Getter&& getter, Setter&& setter);
            LinkGenerator(LinkID id, LinkIDResolver&& idResolver, Getter&& getter, Setter&& setter);

            Link Generate(TypedObjectReference<ClassT> from);
        private:
            Getter getter;
            Setter setter;

            LinkIDResolver idResolver;
        private:
            static_assert(std::is_arithmetic<LinkT>::value && !std::is_const<LinkT>::value, "Links are required to be arithmetic and not constant.");
        };

        template<class ClassT, class LinkT>
        LinkGenerator<ClassT, LinkT>::LinkGenerator(LinkID id, Getter&& getter, Setter&& setter) :
            id(id), getter(std::move(getter)), setter(std::move(setter))
        {}

        template<class ClassT, class LinkT>
        LinkGenerator<ClassT, LinkT>::LinkGenerator(LinkID id, LinkIDResolver&& idResolver, Getter&& getter, Setter&& setter) :
            id(id), idResolver(std::move(idResolver)), getter(std::move(getter)), setter(std::move(setter))
        {}

        template<class ClassT, class LinkT>
        Link LinkGenerator<ClassT, LinkT>::Generate(TypedObjectReference<ClassT> from)
        {
            return Link(getter, setter, from, id);
        }
    }
}