#pragma once

#include <Chroma/EventConnection.h>

namespace Creation
{
    class AnyEventConnection
    {
    public:
        AnyEventConnection() = default;
        template<class... Args>
        AnyEventConnection(const ::Chroma::EventConnection<Args...>& connection);
        template<class... Args>
        AnyEventConnection(::Chroma::EventConnection<Args...>&& connection);
        AnyEventConnection(const AnyEventConnection& arg);
        AnyEventConnection(AnyEventConnection&& arg);

        AnyEventConnection& operator=(const AnyEventConnection& arg);
        AnyEventConnection& operator=(AnyEventConnection&& arg);

        bool operator==(const AnyEventConnection& arg) const;
        bool operator!=(const AnyEventConnection& arg) const;

        template<class... Args>
        void Set(const ::Chroma::EventConnection<Args...>& set);
        template<class... Args>
        void Set(::Chroma::EventConnection<Args...>&& set);

        void Sever();
        bool IsValid() const;
    private:
        class Base
        {
        public:
            virtual ~Base() = 0;

            virtual Base* Clone() const = 0;

            virtual void Sever() = 0;
            virtual bool IsValid() const = 0;
            virtual bool Compare(const AnyEventConnection& arg) const = 0;
        };

        template<class... Args>
        class Derived : public Base
        {
        public:
            typedef ::Chroma::EventConnection<Args...> WrappedT;
            WrappedT wrapped;
        public:
            Derived(const WrappedT& wrapped);
            Derived(WrappedT&& wrapped);
            Derived(const Derived& arg) = default;

            Derived& operator=(const Derived& arg) = delete;

            Derived* Clone() const override;

            void Sever() override final;
            bool IsValid() const override;

            bool Compare(const AnyEventConnection& arg) const override;
        };
    private:
        std::unique_ptr<Base> base;
    };

    template<class... Args>
    AnyEventConnection::Derived<Args...>::Derived(const WrappedT& wrapped) : wrapped(wrapped)
    {}

    template<class... Args>
    AnyEventConnection::Derived<Args...>::Derived(WrappedT&& wrapped) : wrapped(std::move(wrapped))
    {}

    template<class... Args>
    typename AnyEventConnection::Derived<Args...>* AnyEventConnection::Derived<Args...>::Clone() const
    {
        return new Derived(*this);
    }

    template<class... Args>
    void AnyEventConnection::Derived<Args...>::Sever()
    {
        wrapped.Sever();
    }

    template<class... Args>
    bool AnyEventConnection::Derived<Args...>::IsValid() const
    {
        return wrapped.IsValid();
    }

    template<class... Args>
    bool AnyEventConnection::Derived<Args...>::Compare(const AnyEventConnection& arg) const
    {
        return wrapped == static_cast<Derived*>(arg.base.get())->wrapped;
    }

    template<class... Args>
    AnyEventConnection::AnyEventConnection(const ::Chroma::EventConnection<Args...>& connection) : base(new Derived<Args...>(connection))
    {}

    template<class... Args>
    AnyEventConnection::AnyEventConnection(::Chroma::EventConnection<Args...>&& connection) : base(new Derived<Args...>(std::move(connection)))
    {}

    template<class... Args>
    void AnyEventConnection::Set(const ::Chroma::EventConnection<Args...>& set)
    {
        base.reset(new Derived<Args...>(set));
    }

    template<class... Args>
    void AnyEventConnection::Set(::Chroma::EventConnection<Args...>&& set)
    {
        base.reset(new Derived<Args...>(std::move(set)));
    }
}