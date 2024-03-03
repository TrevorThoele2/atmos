#pragma once

#include <vector>

#include "RegistryObject.h"
#include "Registry.h"

#include "Script.h"

#include "Serialization.h"

namespace Atmos
{
    class Quest : public RegistryObject
    {
    public:
        typedef String Name;
        typedef String Description;
    public:
        class Part
        {
        public:
            typedef Quest::Name Name;
            typedef Quest::Description Description;

            enum class Type
            {
                WAR,
                DIPLOMACY,
                GATHER,
                EXPLORE,
                MISC
            };
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        public:
            Name name;
            Description description;

            Type type;

            Part() = default;
            Part(const Name &name, const Description &description, Type type);
        };

        typedef std::vector<Part> Parts;
        typedef Parts::size_type PartID;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        Parts parts;
        Parts::iterator curPos;
        Script::Instance script;

        Quest();
        Quest(const Quest &arg) = default;
        Quest& operator=(const Quest &arg) = default;
        Quest(Quest &&arg);
        Quest& operator=(Quest &&arg);

        bool operator==(const Quest &arg) const;
        bool operator!=(const Quest &arg) const;

        Part* AddPart();
        void RemovePart(PartID id);
        Part* FindPart(PartID id);
    };

    template<>
    class Registry<Quest> : public RegistryBase<Quest, Registry<Quest>>
    {
    private:
        Registry() = default;
        friend RegistryBase<Quest, Registry<Quest>>;
    };

    typedef Registry<Quest> QuestRegistry;
}