
#include "Field.h"

#include "WorldManager.h"
#include "Camera.h"
#include "Debug.h"

#include <Inscription\Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Field)
    {
        scribe(tiles);
        scribe(entities);
        scribe(groundItems);
        scribe(scheduler);
    }

    void Field::SetupVector()
    {
        componentVector.push_back(&tiles);
        componentVector.push_back(&entities);
        componentVector.push_back(&groundItems);
        componentVector.push_back(&renderFragments);
        componentVector.push_back(&lighting);
        componentVector.push_back(&sounds);
        componentVector.push_back(&scheduler);
        componentVector.push_back(&orphanScripts);
    }

    Field::Field(ID id) : id(id), tiles(*this), entities(*this), groundItems(*this), renderFragments(*this), lighting(*this), sounds(*this), scheduler(*this), orphanScripts(*this)
    {
        SetupVector();
    }

    Field::Field(Field &&arg) : id(arg.id), tiles(*this, std::move(arg.tiles)), entities(*this, std::move(arg.entities)), groundItems(*this, std::move(arg.groundItems)), renderFragments(*this, std::move(arg.renderFragments)), lighting(*this, std::move(arg.lighting)), sounds(*this, std::move(arg.sounds)), scheduler(*this, std::move(arg.scheduler)), orphanScripts(*this, std::move(arg.orphanScripts))
    {
        SetupVector();
    }

    Field& Field::operator=(Field &&arg)
    {
        id = std::move(arg.id);
        tiles = std::move(arg.tiles);
        entities = std::move(arg.entities);
        groundItems = std::move(arg.groundItems);
        renderFragments = std::move(arg.renderFragments);
        lighting = std::move(arg.lighting);
        sounds = std::move(arg.sounds);
        scheduler = std::move(arg.scheduler);
        orphanScripts = std::move(arg.orphanScripts);
        return *this;
    }

    Field::ID Field::GetID() const
    {
        return id;
    }

    void Field::OnFinalizeField()
    {
        for (auto &loop : componentVector)
            loop->OnFinalizeField();
    }

    void Field::OnBeforeFieldSet(Field &newField)
    {
        for (auto &loop : componentVector)
            loop->OnBeforeFieldSet(newField);

        GameEnvironment::StopAll();
    }

    void Field::OnAfterFieldSet()
    {
        for (auto &loop : componentVector)
            loop->OnAfterFieldSet();
    }
}