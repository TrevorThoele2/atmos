#include "ScriptCurator.h"

#include "CurrentExecutingScript.h"
#include "ScriptFinished.h"

namespace Atmos::Scripting
{
    Curator::Curator(Init init, Manager& manager) :
        Arca::Curator(init),
        manager(&manager)
    {
        this->manager->Initialize(Owner());

        Owner().On<Arca::DestroyingKnown<Script>>(
            [this](const Arca::DestroyingKnown<Script>& signal)
            {
                const auto currentExecutingScript = Arca::Index<CurrentExecutingScript>(Owner())->id;
                if (currentExecutingScript != signal.index.ID())
                    Owner().Raise(Finished{ signal.index, Quit{} });
            });
    }

    void Curator::Handle(const Work&)
    {
        struct Element
        {
            Arca::RelicID id;
            const Script* script;
            Element(Arca::RelicID id, const Script* script) : id(id), script(script)
            {}
            Element(const Element& arg) = default;
        };

        std::vector<Element> elements;
        const auto batch = Owner().Batch<Script>();
        elements.reserve(batch.Size());
        for (auto script = batch.begin(); script != batch.end(); ++script)
            elements.emplace_back(script.ID(), &*script);

        const auto currentExecutingScript = MutablePointer().Of<CurrentExecutingScript>();

        for(auto& element : elements)
        {
            currentExecutingScript->id = element.id;

            auto script = MutablePointer().Of<Script>(element.id);
            if (!script->Resource())
            {
                auto resource = manager->CreateScriptResource(script->asset->Name(), script->executeName, script->parameters);
                script->Setup(std::move(resource));
            }

            auto result = DoExecute(script);

            if (result)
            {
                Owner().Raise(Finished{ Arca::Index<Script>(element.id, Owner()), *result });
                Owner().Do(Arca::Destroy<Script>(element.id));
            }
        }

        currentExecutingScript->id = Arca::nullRelicID;
    }

    void Curator::Handle(const Suspend& command)
    {
        const auto script = MutablePointer().Of(command.script);
        if (!script || script->isSuspended)
            return;

        script->isSuspended = true;
        script->Resource()->Suspend();
    }

    File::Path Curator::Handle(const Compile& command)
    {
        return manager->Compile(command.inputFilePath, command.outputFilePath);
    }

    void Curator::Handle(const ModifyData& command)
    {
        auto removeSet = std::set<String>(command.remove.begin(), command.remove.end());
        std::unordered_map<String, Variant> replaceMap;
        for (auto& datum : command.replace)
            replaceMap.emplace(datum.name, datum.value);

        auto mutableScript = MutablePointer().Of(command.script);

        for (auto element = mutableScript->data.begin(); element != mutableScript->data.end();)
        {
            if (removeSet.find(element->name) != removeSet.end())
                element = mutableScript->data.erase(element);
            else
            {
                auto foundReplace = replaceMap.find(element->name);
                if (foundReplace != replaceMap.end())
                    element->value = foundReplace->second;
                ++element;
            }
        }
        mutableScript->data.insert(mutableScript->data.begin(), command.add.begin(), command.add.end());
    }

    std::unique_ptr<Asset::Resource::Script> Curator::Handle(
        const Asset::Resource::Create<Asset::Resource::Script>& command)
    {
        return manager->CreateAssetResource(command.buffer, command.name);
    }

    std::optional<Result> Curator::DoExecute(Script* script)
    {
        if (script->isSuspended)
        {
            script->isSuspended = false;
            return script->Resource()->Resume();
        }
        else
            return script->Resource()->Execute();
    }
}