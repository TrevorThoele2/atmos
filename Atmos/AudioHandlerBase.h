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
    public:
        AudioHandlerBase() = default;
        virtual ~AudioHandlerBase() = 0 {}

        std::unique_ptr<AudioAssetData> CreateAudioData(const FilePath& path);
        std::unique_ptr<AudioAssetData> CreateAudioData(void* buffer, SizeT fileSize, const FileName& name);
        virtual bool SetMasterVolume(float setTo) = 0;

        static bool CanMake(const FilePath& path);
        static bool CanMake(void* buffer, SizeT fileSize);
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

        typedef std::pair<BufferT, Format> ExtractedFile;
    private:
        enum class FileType
        {
            NONE,
            WAV,
            OGG
        };
    private:
        AudioHandlerBase(const AudioHandlerBase& arg) = delete;
        AudioHandlerBase& operator=(const AudioHandlerBase& arg) = delete;
    private:
        ExtractedFile ExtractFile(FileType fileType, BufferT&& buffer, SizeT fileSize);
        ExtractedFile ExtractFileWAV(BufferT&& buffer, SizeT fileSize);
        ExtractedFile ExtractFileOGG(BufferT&& buffer, SizeT fileSize);

        static FileType FileTypeOf(const BufferT& buffer, SizeT fileSize);
    private:
        virtual std::unique_ptr<AudioAssetData> CreateAudioDataImpl(ExtractedFile&& extracted, const FileName &name) = 0;
    };
}