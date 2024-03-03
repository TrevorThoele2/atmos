
#pragma once

#include <memory>
#include "FieldID.h"
#include "PlayerParty.h"
#include "Serialization.h"

namespace Atmos
{
    class WorldStartBase
    {
    public:
        // This MUST be called after the field is set
        virtual void Use() = 0;
        virtual FieldID GetFieldID() const = 0;
    };

    class Field;
    class WorldStart : public WorldStartBase
    {
    private:
        void Serialize(inscription::Scribe &scribe);
        INSCRIPTION_ACCESS;
    private:
        FieldID fieldID;
    public:
        WorldStart(FieldID fieldID = 0);

        void Set(FieldID fieldID);
        // This MUST be called after the field is set
        void Use() override;

        FieldID GetFieldID() const override;
    };

    class StasisWorldStart : public WorldStartBase
    {
    public:
        typedef std::array<Entity, Ent::PlayerParty::maxSize> PlayerParty;
    private:
        void Serialize(inscription::Scribe &scribe);
        INSCRIPTION_ACCESS;
    private:
        FieldID fieldID;
        // Outside field will be the same field ID as above if not in battle
        FieldID outsideField;

        PlayerParty playerParty;
    public:
        // Main game constructor
        StasisWorldStart(FieldID fieldID = 0);
        // Battle constructor
        StasisWorldStart(FieldID fieldID, FieldID outsideField);

        void SetFromCurrent();
        // Main game set
        void Set(FieldID fieldID);
        // Battle set
        void Set(FieldID fieldID, FieldID outsideField);
        void SetPlayerParty(const PlayerParty &set);
        // The current field must already be set
        void SetBattleFieldFromCurrent();
        void SetPlayerPartyFromCurrent();
        // This MUST be called after the field is set
        void Use() override;

        FieldID GetFieldID() const override;
        FieldID GetOutsideFieldID() const;
        const PlayerParty& GetPlayerParty() const;
    };
}