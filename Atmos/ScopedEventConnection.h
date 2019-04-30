#pragma once

#include <Chroma/EventConnection.h>

#include "Assert.h"

namespace Atmos
{
    class ScopedEventConnection
    {
    public:
        ScopedEventConnection() = default;
        template<class... Args>
        ScopedEventConnection(const ::Chroma::EventConnection<Args...>& wrapped);
        template<class... Args>
        ScopedEventConnection(::Chroma::EventConnection<Args...>&& wrapped);
        ScopedEventConnection(const ScopedEventConnection& arg);
        ScopedEventConnection(ScopedEventConnection&& arg);
        ~ScopedEventConnection();

        ScopedEventConnection& operator=(const ScopedEventConnection& arg);
        ScopedEventConnection& operator=(ScopedEventConnection&& arg);

        bool operator==(const ScopedEventConnection& arg) const;
        bool operator!=(const ScopedEventConnection& arg) const;

        template<class... Args>
        void Set(const ::Chroma::EventConnection<Args...>& set);
        template<class... Args>
        void Set(::Chroma::EventConnection<Args...>&& set);
        // Does not remove this slot from the event
        void Reset();

        template<class... Args>
        void Execute(Args&& ... args);
        // Removes slot from event
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
        };

        template<class... Args>
        class Derived : public Base
        {
        public:
            typedef typename ::Chroma::Event<Args...>::Connection Wrapped;
            Wrapped wrapped;
        public:
            Derived(const Wrapped &wrapped);
            Derived(Wrapped &&arg);
            ~Derived();

            Derived* Clone() const override;

            void Sever() override;
            bool IsValid() const override;
        };
    private:
        std::unique_ptr<Base> base;
    };

    template<class... Args>
    ScopedEventConnection::Derived<Args...>::Derived(const Wrapped& wrapped) : wrapped(wrapped)
    {}

    template<class... Args>
    ScopedEventConnection::Derived<Args...>::Derived(Wrapped&& wrapped) : wrapped(std::move(wrapped))
    {}

    template<class... Args>
    ScopedEventConnection::Derived<Args...>::~Derived()
    {
        Sever();
    }

    template<class... Args>
    ScopedEventConnection::Derived<Args...>* ScopedEventConnection::Derived<Args...>::Clone() const
    {
        return new Derived(*this);
    }

    template<class... Args>
    void ScopedEventConnection::Derived<Args...>::Sever()
    {
        if (IsValid())
            wrapped.Sever();
    }

    template<class... Args>
    bool ScopedEventConnection::Derived<Args...>::IsValid() const
    {
        return wrapped.IsValid();
    }

    template<class... Args>
    ScopedEventConnection::ScopedEventConnection(const ::Chroma::EventConnection<Args...>& wrapped) : base(new Derived<Args...>(wrapped))
    {}

    template<class... Args>
    ScopedEventConnection::ScopedEventConnection(::Chroma::EventConnection<Args...>&& wrapped) : base(new Derived<Args...>(std::move(wrapped)))
    {}

    template<class... Args>
    void ScopedEventConnection::Set(const ::Chroma::EventConnection<Args...>& set)
    {
        base.reset(new Derived<Args...>(set));
    }

    template<class... Args>
    void ScopedEventConnection::Set(::Chroma::EventConnection<Args...>&& set)
    {
        base.reset(new Derived<Args...>(std::move(set)));
    }

    template<class... Args>
    void ScopedEventConnection::Execute(Args&& ... args)
    {
        auto converted = dynamic_cast<Derived<Args...>*>(base.get());
        ATMOS_ASSERT(converted != nullptr);
        converted->wrapped.Execute(std::forward<Args>(args)...);
    }
}