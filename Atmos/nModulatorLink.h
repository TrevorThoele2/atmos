#pragma once

#include "ModulatorValue.h"
#include "nModulatorLinkID.h"

#include "ObjectReference.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    namespace Modulator
    {
        class Link
        {
        public:
            template<class ClassT>
            using Getter = std::function<Value(TypedObjectReference<ClassT>)>;
            template<class ClassT, class LinkT>
            using Setter = std::function<void(TypedObjectReference<ClassT>, LinkT)>;
        public:
            Link();
            template<class ClassT, class LinkT>
            Link(const Getter<ClassT>& getter, const Setter<ClassT, LinkT>& setter, TypedObjectReference<ClassT> object, LinkID id);
            Link(const ::Inscription::Table<Link>& table);
            Link(const Link& arg);
            Link(Link&& arg);

            Link& operator=(const Link& arg);
            Link& operator=(Link&& arg);

            void SetValue(const Value& value);
            Value GetValue() const;

            Value::Type Type() const;
        private:
            class Base
            {
            public:
                virtual ~Base() = 0;

                virtual Base* Clone() const = 0;

                virtual void SetValue(const Value& value) = 0;
                virtual Value GetValue() const = 0;

                virtual Value::Type Type() const = 0;
            };

            template<class ClassT, class LinkT>
            class Derived : public Base
            {
            public:
                Derived(const Getter<ClassT>& getter, const Setter<ClassT, LinkT>& setter, TypedObjectReference<ClassT> object);

                Derived* Clone() const override;

                void SetValue(const Value& value) override;
                Value GetValue() const override;

                Value::Type Type() const override;
            private:
                TypedObjectReference<ClassT> object;
            private:
                Getter<ClassT> getter;
                Setter<ClassT, LinkT> setter;
            };
        private:
            LinkID id;
            ObjectReference object;
        private:
            typedef std::unique_ptr<Base> BasePtr;
            BasePtr base;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        };

        template<class ClassT, class LinkT>
        Link::Link(const Getter<ClassT>& getter, const Setter<ClassT, LinkT>& setter, TypedObjectReference<ClassT> object, LinkID id) :
            base(new Derived<ClassT, LinkT>(getter, setter, object)), object(object), id(id)
        {}

        template<class ClassT, class LinkT>
        Link::Derived<ClassT, LinkT>::Derived(const Getter<ClassT>& getter, const Setter<ClassT, LinkT>& setter, TypedObjectReference<ClassT> object) :
            getter(getter), setter(setter), object(object)
        {}

        template<class ClassT, class LinkT>
        Link::Derived<ClassT, LinkT>* Link::Derived<ClassT, LinkT>::Clone() const
        {
            return new Derived(*this);
        }

        template<class ClassT, class LinkT>
        void Link::Derived<ClassT, LinkT>::SetValue(const Value& value)
        {
            auto bytes = value.AsBytes();
            setter(object, *reinterpret_cast<LinkT*>(bytes.data()));
        }

        template<class ClassT, class LinkT>
        Value Link::Derived<ClassT, LinkT>::GetValue() const
        {
            return getter(object);
        }

        template<class ClassT, class LinkT>
        Value::Type Link::Derived<ClassT, LinkT>::Type() const
        {
            if (std::is_floating_point<LinkT>::value)
                return Value::Type::FLOAT;
            else
                return Value::Type::INT;
        }
    }
}