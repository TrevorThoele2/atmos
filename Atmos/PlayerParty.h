#pragma once

#include <array>

#include "EntitySystem.h"

#include "Name.h"
#include "BattleCharacter.h"

#include "Serialization.h"

namespace Atmos
{
    namespace Ent
    {
        class PlayerParty : public System<PlayerParty>
        {
        public:
            typedef Atmos::Battle::CharacterEntity PlayerT;

            typedef unsigned char SizeT;
            static constexpr SizeT maxSize = 16;
            static constexpr SizeT squadCount = 4;
            static constexpr SizeT squadSize = maxSize / squadCount;

            typedef std::array<PlayerT, squadSize> Squad;
            typedef std::array<Squad, squadCount> SquadArray;
        private:
            typedef System<PlayerParty> SystemBaseT;
            friend SystemBaseT;

            PlayerParty();
            PlayerParty(const PlayerParty &arg) = delete;
            PlayerParty& operator=(const PlayerParty &arg) = delete;

            void Init() override;

            void OnFieldSet(Field &field);

            void ResetSquads();
            static bool IsOutOfRange(SizeT position);
            static bool IsOutOfRange(SizeT squad, SizeT position);
        public:
            SquadArray squads;

            // This will configure the entity for the party
            // This MUST conform to the battle character package
            // It will add in the battle component if needed
            static Entity Add(Entity add);
            static bool Erase(SizeT position);
            static bool Erase(SizeT squad, SizeT position);

            static Entity Find(const Name &name);
            static Entity Find(SizeT position);
            static Entity Find(SizeT squad, SizeT position);

            static bool Move(SizeT from, SizeT to);
            static bool Move(SizeT squadFrom, SizeT positionFrom, SizeT squadTo, SizeT positionTo);

            static bool IsHere(Entity entity);
            static bool CanAddMore();
        };
    }
}