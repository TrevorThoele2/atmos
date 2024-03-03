
#include "Quest.h"

#include <Inscription/Scribe.h>
#include <Inscription/Vector.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Quest::Part)
    {
        scribe(name);
        scribe(description);
        scribe(type);
    }

    Quest::Part::Part(const Name &name, const Description &description, Type type) : name(name), description(description), type(type)
    {}

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Quest)
    {
        ::Inscription::BaseSerialize<RegistryObject>(scribe, *this);

        if (scribe.IsOutput())
        {
            scribe.Save(name);
            scribe.Save(description);
            scribe.Save(parts);
            scribe.Save(script);

            // Save relative position to the current position
            size_t position = curPos - parts.begin();
            scribe.Save(position);
        }
        else
        {
            scribe.Load(name);
            scribe.Load(description);
            scribe.Load(parts);
            scribe.Load(script);

            // Load relative position to the current position
            size_t position;
            scribe.Load(position);
            curPos = parts.begin() + position;
        }
    }

    Quest::Quest() : curPos(parts.end())
    {}

    Quest::Quest(Quest &&arg) : RegistryObject(std::move(arg)), parts(std::move(arg.parts)), curPos(std::move(arg.curPos)), script(std::move(arg.script))
    {}

    Quest& Quest::operator=(Quest &&arg)
    {
        RegistryObject::operator=(std::move(arg));
        name = std::move(arg.name);
        description = std::move(arg.description);
        parts = std::move(arg.parts);
        curPos = std::move(arg.curPos);
        script = std::move(arg.script);

        return *this;
    }

    bool Quest::operator==(const Quest &arg) const
    {
        return RegistryObject::operator==(arg);
    }

    bool Quest::operator!=(const Quest &arg) const
    {
        return !(*this == arg);
    }

    Quest::Part* Quest::AddPart()
    {
        parts.push_back(Part());
        return &parts.back();
    }

    void Quest::RemovePart(PartID id)
    {
        parts.erase(parts.begin() + id);
    }

    Quest::Part* Quest::FindPart(PartID id)
    {
        if (parts.size() <= id)
            return nullptr;

        return &parts[id];
    }
}