#pragma once

#include "JavaScriptType.h"
#include "UniversalSound.h"

#include "JavaScriptAudioAsset.h"
#include "JavaScriptObject.h"

#include <Arca/Create.h>

#include <Inscription/OptionalScribe.h>

namespace Atmos::Scripting::JavaScript
{
    struct UniversalSound
    {
        Arca::RelicID id;
        std::optional<AudioAsset> asset;
        Audio::Volume volume;
    };

    template<>
    class Type<UniversalSound>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const UniversalSound& value);
        [[nodiscard]] static std::optional<UniversalSound> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
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
    class Type<Arca::Index<Audio::UniversalSound>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Audio::UniversalSound>& value);
    };

    struct CreateUniversalSound
    {
        std::optional<AudioAsset> asset;
        Audio::Volume volume;
    };

    template<>
    class Type<CreateUniversalSound>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<CreateUniversalSound> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<std::optional<AudioAsset>> asset;
            Property<Audio::Volume> volume;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Create<Audio::UniversalSound>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<Arca::Create<Audio::UniversalSound>> FromV8(
            v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::UniversalSound> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::UniversalSound;
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
    struct ScribeTraits<Atmos::Scripting::JavaScript::UniversalSound, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::UniversalSound>;
    };

    template<>
    class Scribe<Atmos::Scripting::JavaScript::CreateUniversalSound> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::CreateUniversalSound;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("asset", object.asset);
            format("volume", object.volume);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::CreateUniversalSound, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::CreateUniversalSound>;
    };
}