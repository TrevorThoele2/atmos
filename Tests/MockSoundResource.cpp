#include "MockSoundResource.h"

void MockSoundResource::Restart()
{
    ++restartCount;
}

void MockSoundResource::Pause()
{
    ++pauseCount;
}

void MockSoundResource::Resume()
{
    ++resumeCount;
}

void MockSoundResource::SetVolume(Audio::Volume volume)
{
    ++setVolumeCount;
}

void MockSoundResource::SetPosition(const Spatial::Point2D& position)
{
    ++setPositionCount;
}