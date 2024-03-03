#pragma once

#include <Atmos/SoundResource.h>

using namespace Atmos;

class MockSoundResource final : public Audio::Resource::Sound
{
public:
    int restartCount = 0;
    int pauseCount = 0;
    int resumeCount = 0;
    int setVolumeCount = 0;
    int setPositionCount = 0;
public:
    void Restart() override;
    void Pause() override;
    void Resume() override;
    void SetVolume(Audio::Volume volume) override;
    void SetPosition(const Spatial::Point2D& position) override;
};