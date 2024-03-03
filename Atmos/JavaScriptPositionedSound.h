#pragma once

#include "JavaScriptType.h"
#include "PositionedSound.h"

#include "JavaScriptAudioAsset.h"
#include "JavaScriptObject.h"

#include <Arca/Create.h>

#include <Inscription/OptionalScribe.h>

namespace Atmos::Scripting::JavaScript
{
    struct PositionedSound
    {
        Arca::RelicID id;
        std::optional<AudioAsset> asset;
        Audio::Volume volume;
    };

    template<>
    class Type<PositionedSound>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const PositionedSound& value);
        [[nodiscard]] static std::optional<PositionedSound> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<std::optional<AudioAsset>> asset;
            Property<Audio::Volume> volume;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Audio::PositionedSound>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Audio::PositionedSound>& value);
    };

    struct CreatePositionedSound
    {
        std::optional<AudioAsset> asset;
        Audio::Volume volume;
        Spatial::Point3D position;
    };

    template<>
    class Type<CreatePositionedSound>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<CreatePositionedSound> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<std::optional<AudioAsset>> asset;
            Property<Audio::Volume> volume;
            Property<Spatial::Point3D> position;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Create<Audio::PositionedSound>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<Arca::Create<Audio::PositionedSound>> FromV8(
            v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::PositionedSound> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::PositionedSound;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("asset", object.asset);
            format("volume", object.volume);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::PositionedSound, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::PositionedSound>;
    };

    template<>
    class Scribe<Atmos::Scripting::JavaScript::CreatePositionedSound> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::CreatePositionedSound;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("asset", object.asset);
            format("volume", object.volume);
            format("position", object.position);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::CreatePositionedSound, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::CreatePositionedSound>;
    };
}