
#pragma once

#include <memory>
#include "FieldID.h"
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
    public:
        WorldStart(FieldID fieldID = 0);

        void Set(FieldID fieldID);
        // This MUST be called after the field is set
        void Use() override;

        FieldID GetFieldID() const override;
    private:
        FieldID fieldID;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };

    class StasisWorldStart : public WorldStartBase
    {
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
        // The current field must already be set
        void SetBattleFieldFromCurrent();
        void SetPlayerPartyFromCurrent();
        // This MUST be called after the field is set
        void Use() override;

        FieldID GetFieldID() const override;
        FieldID GetOutsideFieldID() const;
    private:
        FieldID fieldID;
        // Outside field will be the same field ID as above if not in battle
        FieldID outsideField;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}