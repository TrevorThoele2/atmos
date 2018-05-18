#pragma once

#include <unordered_set>

#include "FieldComponent.h"
#include "Event.h"

#include "Serialization.h"

namespace Atmos
{
    class Sound;
    class SoundHandler : public FieldComponent
    {
    private:
        typedef std::unordered_set<Sound*> Set;
        Set container;

        EventScopedConnectionManager eventSlots;

        SoundHandler(const SoundHandler &arg) = delete;
        SoundHandler& operator=(const SoundHandler &arg) = delete;

        void OnCameraMoved();

        void SetVolumeByCameraPos();
        void Add(Sound &sound);
        void Remove(Sound &sound);

        friend Sound;
    public:
        SoundHandler(Field &field);
        SoundHandler(Field &field, SoundHandler &&arg);
        SoundHandler& operator=(SoundHandler &&arg);
        void Work();

        void ResumeAll();
        void PauseAll();
    };
}