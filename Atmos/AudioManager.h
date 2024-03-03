#pragma once

#include "AudioAsset.h"

#include "Volume.h"
#include "Buffer.h"

namespace Atmos::Audio
{
    class AudioManager
    {
    public:
        AudioManager(const AudioManager& arg) = delete;
        AudioManager& operator=(const AudioManager& arg) = delete;
        virtual ~AudioManager() = 0;

        std::unique_ptr<Asset::Resource::Audio> CreateAudioResource(const Buffer& buffer, const Name& name);
        virtual bool SetMasterVolume(Volume setTo) = 0;
    protected:
        AudioManager();
    protected:
        struct Format
        {
            std::int16_t formatTag;
            std::int16_t channels;
            std::int32_t samplesPerSec;
            std::int16_t bitsPerSample;
            std::int16_t blockAlign;
            std::int32_t avgBytesPerSec;
        };

        struct FormattedBuffer
        {
            Buffer buffer;
            Format format;
        };
    private:
        enum class FileType
        {
            Unknown,
            Wav,
            Ogg
        };
    private:
        static FormattedBuffer FormatBuffer(const Buffer& buffer, const Name& name);
        static FormattedBuffer FormatBufferWAV(const Buffer& buffer);
        static FormattedBuffer FormatBufferOGG(const Buffer& buffer);

        static FileType FileTypeOf(const Buffer& buffer);
    private:
        virtual std::unique_ptr<Asset::Resource::Audio> CreateAudioResourceImpl(
            const FormattedBuffer& buffer, const Name& name) = 0;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Audio::AudioManager>
    {
        static TypeName TypeName() { return "Atmos::Audio::AudioManager"; }
    };
}