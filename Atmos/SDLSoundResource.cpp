#include "SDLSoundResource.h"

#include "AudioError.h"

#include "SpatialAlgorithms.h"
#include "MathUtility.h"

namespace Atmos::Audio::Resource::SDL
{
    Sound::Sound(SDL_RWops* ops, Atmos::Audio::SDL::Channel channel, Volume volume) : channel(channel)
    {
        LoadChunk(ops);
        if (!chunk)
            throw Error("Could not create audio resource.", { {"Reason", String(SDL_GetError())} });

        SetChunkVolume(volume);
        StartChunk();
    }

    Sound::Sound(Sound&& arg) noexcept : chunk(arg.chunk)
    {}

    Sound::~Sound()
    {
        if (chunk)
        {
            StopChunk();
            FreeChunk();
        }
    }

    Sound& Sound::operator=(Sound&& arg) noexcept
    {
        chunk = arg.chunk;
        return *this;
    }

    void Sound::Restart()
    {
        StopChunk();
        StartChunk();
    }

    void Sound::Pause()
    {
        PauseChunk();
    }

    void Sound::Resume()
    {
        ResumeChunk();
    }

    void Sound::SetVolume(Volume volume)
    {
        SetChunkVolume(volume);
    }

    void Sound::SetPosition(const Spatial::Point2D& position)
    {
        SetChunkPosition(position);
    }

    void Sound::LoadChunk(SDL_RWops* ops)
    {
        chunk = Mix_LoadWAV_RW(ops, 0);
    }

    void Sound::FreeChunk()
    {
        Mix_FreeChunk(chunk);
    }

    void Sound::StartChunk()
    {
        Mix_PlayChannel(channel, chunk, 0);
    }

    void Sound::StopChunk()
    {
        Mix_HaltChannel(channel);
    }

    void Sound::PauseChunk()
    {
        Mix_Pause(channel);
    }

    void Sound::ResumeChunk()
    {
        Mix_Resume(channel);
    }

    void Sound::SetChunkVolume(Volume volume)
    {
        const auto clampedVolume = std::clamp(volume, 0.0f, 1.0f);
        const auto mixVolume = static_cast<int>(ConvertRange(clampedVolume, 0.0f, 1.0f, minMixVolume, maxMixVolume));

        Mix_VolumeChunk(chunk, mixVolume);
    }

    void Sound::SetChunkPosition(const Spatial::Point2D& position)
    {
        const auto max = std::numeric_limits<Spatial::Point2D::Value>::max();

        const auto standardPosition = Spatial::Point2D{ 0, max };
        const auto origin = Spatial::Point2D{ 0, 0 };
        const auto angle = Spatial::AngleOf(standardPosition, origin, position);

        const auto fullCircle = pi<Spatial::Angle2D> * 2;
        auto passAngle = angle;
        while (passAngle < 0)
            passAngle += fullCircle;
        while (passAngle > fullCircle)
            passAngle -= fullCircle;
        const auto mixAngle = static_cast<Uint16>(Spatial::ToDegrees(passAngle));

        const auto distance = Spatial::Distance(origin, position);
        const auto clampedDistance = std::min(distance, maxDistance);
        const auto mixDistance = static_cast<Uint8>(ConvertRange(clampedDistance, 0.0f, maxDistance, minMixDistance, maxMixDistance));

        Mix_SetPosition(channel, mixAngle, mixDistance);
    }
}