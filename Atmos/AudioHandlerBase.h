#pragma once

#include "AudioAsset.h"
#include "FilePath.h"
#include "Buffer.h"

namespace Atmos
{
    class AudioHandlerBase
    {
    public:
        typedef std::uint32_t SizeT;
    private:
        enum class Type
        {
            NONE,
            WAV,
            OGG
        };
    protected:
        typedef Buffer<SizeT> BufferT;

        struct Format
        {
            std::int16_t formatTag;
            std::int16_t channels;
            std::int32_t samplesPerSec;
            std::int16_t bitsPerSample;
            std::int16_t blockAlign;
            std::int32_t avgBytesPerSec;
        };

        typedef std::pair<BufferT, Format> Data;
    private:
        AudioHandlerBase(const AudioHandlerBase &arg) = delete;
        AudioHandlerBase& operator=(const AudioHandlerBase &arg) = delete;

        Data CreateData(Type type, void *buffer, SizeT fileSize);
        Data CreateDataWAV(void *buffer, SizeT fileSize);
        Data CreateDataOGG(void *buffer, SizeT fileSize);
        static Type GetType(void *buffer, SizeT fileSize);
        virtual AudioAsset CreateAudioImpl(Data &&data, const FileName &name) = 0;
    public:
        AudioHandlerBase() = default;
        virtual ~AudioHandlerBase() = 0 {}
        AudioAsset CreateAudio(const FilePath &path);
        AudioAsset CreateAudio(void *buffer, SizeT fileSize, const FileName &name);
        virtual bool SetMasterVolume(float setTo) = 0;

        static bool CanMake(const FilePath &path);
        static bool CanMake(void *buffer, SizeT fileSize);
    };
}