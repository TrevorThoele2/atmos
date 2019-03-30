#pragma once

#include "nModulatorLink.h"
#include "nModulatorLinkGeneratorGroup.h"

#include "Object.h"
#include "ObjectTypeName.h"

namespace Atmos
{
    namespace Modulator
    {
        class LinkMap
        {
        public:
            Link Create(LinkID id, ObjectReference from);

            template<class T>
            void Register();
        private:
            class Base
            {
            public:
                virtual ~Base() = 0;

                virtual Link Create(LinkID id, ObjectReference from) = 0;
            };

            template<class T>
            class Derived : public Base
            {
            public:
                Link Create(LinkID id, ObjectReference from) override;
            private:
                typedef LinkGeneratorGroup<T> GroupT;
            };
        private:
            typedef std::unique_ptr<Base> BasePtr;
            typedef std::unordered_map<ObjectTypeName, BasePtr> BaseMap;
            BaseMap bases;
        };

        template<class T>
        void LinkMap::Register()
        {
            bases.emplace(TypeNameFor<T>(), std::make_unique<Derived<T>>());
        }

        template<class T>
        Link LinkMap::Derived<T>::Create(LinkID id, ObjectReference from)
        {
            return GroupT::Create(id, from.Downcast<T>());
        }
    }
}