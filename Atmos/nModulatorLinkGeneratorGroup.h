#pragma once

#include "nModulatorLinkGenerator.h"

#include "ObjectReference.h"

namespace Atmos
{
    namespace Modulator
    {
        template<class ClassT>
        struct LinkGeneratorGroup;

        template<class ClassT>
        class LinkGeneratorGroupBase
        {
        public:
            template<class LinkT>
            using Generator = LinkGenerator<ClassT, LinkT>;

            typedef ClassT Managed;
        public:
            static LinkGeneratorGroup<ClassT>& Instance();

            static Link Create(LinkID id, TypedObjectReference<ClassT> from);
        protected:
            template<class LinkT>
            static void RegisterGenerator(Generator<LinkT>& generator);
        private:
            class Base
            {
            public:
                virtual ~Base() = 0;

                virtual Link Create(TypedObjectReference<ClassT> from) = 0;
            };

            template<class LinkT>
            class Derived : public Base
            {
            public:
                typedef Generator<LinkT> Generator;
            public:
                Generator& generator;
            public:
                Derived(Generator& generator);

                Link Create(TypedObjectReference<ClassT> from) override;
            };
        private:
            typedef std::unique_ptr<Base> BasePtr;
            typedef std::unordered_map<LinkID, BasePtr> BaseMap;
            static BaseMap bases;
        };

        template<class ClassT>
        LinkGeneratorGroup<ClassT>& LinkGeneratorGroupBase<ClassT>::Instance()
        {
            static LinkGeneratorGroup<ClassT> instance;
            return instance;
        }

        template<class ClassT>
        Link LinkGeneratorGroupBase<ClassT>::Create(LinkID id, TypedObjectReference<ClassT> from)
        {
            return bases.find(id)->second.Create(from);
        }

        template<class ClassT>
        template<class LinkT>
        void LinkGeneratorGroupBase<ClassT>::RegisterGenerator(Generator<LinkT>& generator)
        {
            bases.emplace(generator.id, std::make_unique<Derived<LinkT>>(generator));
        }

        template<class ClassT>
        LinkGeneratorGroupBase<ClassT>::Base::~Base()
        {}

        template<class ClassT>
        template<class LinkT>
        LinkGeneratorGroupBase<ClassT>::Derived<LinkT>::Derived(Generator& generator) : generator(generator)
        {}

        template<class ClassT>
        template<class LinkT>
        Link LinkGeneratorGroupBase<ClassT>::Derived<LinkT>::Create(TypedObjectReference<ClassT> from)
        {
            return generator.Generate(from);
        }

        template<class ClassT>
        typename LinkGeneratorGroupBase<ClassT>::BaseMap LinkGeneratorGroupBase<ClassT>::bases;
    }
}