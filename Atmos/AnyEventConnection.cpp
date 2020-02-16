#include "AnyEventConnection.h"

namespace Atmos
{
    AnyEventConnection::AnyEventConnection(const AnyEventConnection& arg) : base(arg.base->Clone())
    {}

    AnyEventConnection::AnyEventConnection(AnyEventConnection&& arg) noexcept : base(std::move(arg.base))
    {}

    AnyEventConnection& AnyEventConnection::operator=(const AnyEventConnection& arg)
    {
        base = arg.base->Clone();
        return *this;
    }

    AnyEventConnection& AnyEventConnection::operator=(AnyEventConnection&& arg) noexcept
    {
        base = std::move(arg.base);
        return *this;
    }

    bool AnyEventConnection::operator==(const AnyEventConnection& arg) const
    {
        if (!base || !arg.base)
            return base == arg.base;

        if (typeid(base.get()) != typeid(arg.base.get()))
            return false;

        return base->Compare(arg);
    }

    bool AnyEventConnection::operator!=(const AnyEventConnection& arg) const
    {
        return !(*this == arg);
    }

    void AnyEventConnection::Sever()
    {
        if (!base)
            return;

        base->Sever();
    }

    bool AnyEventConnection::IsValid() const
    {
        if (!base)
            return false;

        return base->IsValid();
    }

    AnyEventConnection::Base::~Base() = default;
}