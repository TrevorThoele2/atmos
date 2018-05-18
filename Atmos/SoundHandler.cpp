
#include "SoundHandler.h"

#include "Sound.h"
#include "Camera.h"
#include "WorldManager.h"

namespace Atmos
{
    void SoundHandler::OnCameraMoved()
    {
        SetVolumeByCameraPos();
    }

    void SoundHandler::SetVolumeByCameraPos()
    {
        for(auto &loop : container)
            loop->SetVolumeByCameraPos();
    }

    void SoundHandler::Add(Sound &sound)
    {
        container.emplace(&sound);
    }

    void SoundHandler::Remove(Sound &sound)
    {
        container.erase(&sound);
    }

    SoundHandler::SoundHandler(Field &field) : FieldComponent(field)
    {
        eventSlots.Add(Camera::Instance().eventMoved.Subscribe(&SoundHandler::OnCameraMoved, *this));
    }

    SoundHandler::SoundHandler(Field &field, SoundHandler &&arg) : FieldComponent(field), container(std::move(arg.container))
    {
        eventSlots.Add(Camera::Instance().eventMoved.Subscribe(&SoundHandler::OnCameraMoved, *this));
    }

    SoundHandler& SoundHandler::operator=(SoundHandler &&arg)
    {
        container = std::move(arg.container);
        return *this;
    }

    void SoundHandler::Work()
    {}

    void SoundHandler::ResumeAll()
    {
        for (auto &loop : container)
            loop->Resume();
    }

    void SoundHandler::PauseAll()
    {
        for (auto &loop : container)
            loop->Pause();
    }
}