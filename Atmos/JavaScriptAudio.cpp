#include "JavaScriptAudio.h"

#include "JavaScriptChangeSoundVolume.h"
#include "JavaScriptPauseSound.h"
#include "JavaScriptPositionedSound.h"
#include "JavaScriptRestartSound.h"
#include "JavaScriptResumeSound.h"
#include "JavaScriptSoundFinished.h"
#include "JavaScriptUniversalSound.h"

#include "JavaScriptTraits.h"

#include <Arca/Destroy.h>

namespace Atmos::Scripting::JavaScript
{
    v8::Local<v8::Value> Type<AudioNamespace>::Traits(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                { "ChangeSoundVolume", ToV8(*isolate, ArcaTraits::From<Audio::ChangeSoundVolume>()) },
                { "PauseSound", ToV8(*isolate, ArcaTraits::From<Audio::PauseSound>()) },
                { "RestartSound", ToV8(*isolate, ArcaTraits::From<Audio::RestartSound>()) },
                { "ResumeSound", ToV8(*isolate, ArcaTraits::From<Audio::ResumeSound>()) },
                { "SoundFinished", ToV8(*isolate, ArcaTraits::From<Audio::SoundFinished>()) },
                { "PositionedSound", ToV8(*isolate, ArcaTraits::From<Audio::PositionedSound>()) },
                { "CreatePositionedSound", ToV8(*isolate, ArcaTraits::From<Arca::Create<Audio::PositionedSound>>()) },
                { "DestroyPositionedSound", ToV8(*isolate, ArcaTraits::From<Arca::Destroy<Audio::PositionedSound>>()) },
                { "UniversalSound", ToV8(*isolate, ArcaTraits::From<Audio::UniversalSound>()) },
                { "CreateUniversalSound", ToV8(*isolate, ArcaTraits::From<Arca::Create<Audio::UniversalSound>>()) },
                { "DestroyUniversalSound", ToV8(*isolate, ArcaTraits::From<Arca::Destroy<Audio::UniversalSound>>()) },
            });
    }
}