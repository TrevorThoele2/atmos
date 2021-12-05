#pragma once

#include "JavaScriptType.h"
#include "Entity.h"

#include "JavaScriptAudioAsset.h"
#include "JavaScriptObject.h"

#include <Arca/Create.h>

#include <Inscription/OptionalScribe.h>

namespace Atmos::Scripting::JavaScript
{
    struct Entity
    {
        Arca::RelicID id;
        String name;
        Spatial::Grid::Point position;
        bool isSolid = false;
        std::vector<Tag> tags;
    };

    template<>
    class Type<Entity>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Entity& value);
        [[nodiscard]] static std::optional<Entity> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<String> name;
            Property<Spatial::Grid::Point> position;
            Property<bool> isSolid;
            Property<std::vector<Tag>> tags;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Atmos::Entity::Entity>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Atmos::Entity::Entity>& value);
    };

    struct CreateEntity
    {
        String name;
        Spatial::Grid::Point position;
        bool isSolid = false;
        std::vector<Tag> tags;
    };

    template<>
    class Type<CreateEntity>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<CreateEntity> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<String> name;
            Property<Spatial::Grid::Point> position;
            Property<bool> isSolid;
            Property<std::vector<Tag>> tags;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Create<Atmos::Entity::Entity>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<Arca::Create<Atmos::Entity::Entity>> FromV8(
            v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::Entity> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::Entity;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("name", object.name);
            format("position", object.position);
            format("isSolid", object.isSolid);
            format("tags", object.tags);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::Entity, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::Entity>;
    };

    template<>
    class Scribe<Atmos::Scripting::JavaScript::CreateEntity> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::CreateEntity;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("name", object.name);
            format("position", object.position);
            format("isSolid", object.isSolid);
            format("tags", object.tags);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::CreateEntity, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::CreateEntity>;
    };
}