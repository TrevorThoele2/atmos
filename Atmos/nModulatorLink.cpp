
#include "nModulatorLink.h"

#include "GameEnvironment.h"

namespace Atmos
{
    namespace Modulator
    {
        Link::Link() : id(-1)
        {}

        Link::Link(const ::Inscription::Table<Link>& table)
        {}

        Link::Link(const Link& arg) : base(arg.base->Clone())
        {}

        Link::Link(Link&& arg) : base(std::move(arg.base))
        {}

        Link& Link::operator=(const Link& arg)
        {
            base.reset(arg.base->Clone());
            return *this;
        }

        Link& Link::operator=(Link&& arg)
        {
            base = std::move(arg.base);
            return *this;
        }

        void Link::SetValue(const Value& value)
        {
            base->SetValue(value);
        }

        Value Link::GetValue() const
        {
            return base->GetValue();
        }

        Value::Type Link::Type() const
        {
            return base->Type();
        }

        Link::Base::~Base()
        {}

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Link)
        {
            if (scribe.IsOutput())
            {
                scribe.Save(id);
                scribe.Save(object);
            }
            else // INPUT
            {
                LinkID pulledID;
                scribe.Load(pulledID);

                ObjectReference pulledObject;
                scribe.Load(pulledObject);

                *this = GameEnvironment::GetModulatorLinkMap().Create(pulledID, pulledObject);
            }
        }
    }
}