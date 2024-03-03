
#include "WorldStart.h"

#include "AvatarSystem.h"
#include "WorldManager.h"
#include "Battle.h"
#include "GameEnvironment.h"

#include <Inscription\Scribe.h>
#include <Inscription\String.h>
#include <Inscription\Array.h>

namespace Atmos
{
    WorldStart::WorldStart(FieldID fieldID) : fieldID(fieldID)
    {}

    void WorldStart::Set(FieldID fieldID)
    {
        this->fieldID = fieldID;
    }

    void WorldStart::Use()
    {}

    FieldID WorldStart::GetFieldID() const
    {
        return fieldID;
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(WorldStart)
    {
        scribe(fieldID);
    }

    StasisWorldStart::StasisWorldStart(FieldID fieldID) : fieldID(fieldID), outsideField(fieldID)
    {}

    StasisWorldStart::StasisWorldStart(FieldID fieldID, FieldID outsideField) : fieldID(fieldID), outsideField(outsideField)
    {}

    void StasisWorldStart::SetFromCurrent()
    {
        Set(GameEnvironment::GetWorldManager().GetCurrentField()->GetID());
        SetPlayerPartyFromCurrent();
    }

    void StasisWorldStart::Set(FieldID fieldID)
    {
        this->fieldID = fieldID;
        this->outsideField = fieldID;
    }

    void StasisWorldStart::Set(FieldID fieldID, FieldID outsideField)
    {
        this->fieldID = fieldID;
        this->outsideField = outsideField;
    }

    void StasisWorldStart::SetBattleFieldFromCurrent()
    {
        //outsideField = (battleState.IsTop()) ? battleState.GetOriginalFieldID() : fieldID;
    }

    void StasisWorldStart::SetPlayerPartyFromCurrent()
    {
        /*
        for (auto &squadLoop : Ent::PlayerParty::Instance().squads)
        {
            for (auto &indivSquadLoop : squadLoop)
            {
                for (auto &thisLoop : playerParty)
                    thisLoop = indivSquadLoop.GetEntity();
            }
        }
        */
    }

    void StasisWorldStart::Use()
    {
        if (fieldID != outsideField)
        {
            // Inside battle field
            //battleState.ReinstateFromStasis();
            //battleState.Goto();
        }

        //for (auto& loop : playerParty)
            //Ent::PlayerParty::Add(loop);
    }

    FieldID StasisWorldStart::GetFieldID() const
    {
        return fieldID;
    }

    FieldID StasisWorldStart::GetOutsideFieldID() const
    {
        return outsideField;
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(StasisWorldStart)
    {
        scribe(fieldID);
        scribe(outsideField);
    }
}