#pragma once

#include "AudioAsset.h"

#include "FilePath.h"
#include "Buffer.h"

namespace Atmos::Audio
{
    class AudioManager
    {
    public:
        typedef std::uint32_t SizeT;
    public:
        virtual ~AudioManager() = 0;

        std::unique_ptr<Asset::AudioAssetData> CreateAudioData(const File::Path& path);
        std::unique_ptr<Asset::AudioAssetData> CreateAudioData(void* buffer, SizeT fileSize, const File::Name& name);
        virtual bool SetMasterVolume(float setTo) = 0;

        static bool CanMake(const File::Path& path);
        static bool CanMake(void* buffer, SizeT fileSize);
    protected:
        AudioManager();
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
        AudioManager(const AudioManager& arg) = delete;
        AudioManager& operator=(const AudioManager& arg) = delete;
    private:
        ExtractedFile ExtractFile(FileType fileType, BufferT&& buffer, SizeT fileSize);
        ExtractedFile ExtractFileWAV(BufferT&& buffer, SizeT fileSize);
        ExtractedFile ExtractFileOGG(BufferT&& buffer, SizeT fileSize);

        static FileType FileTypeOf(const BufferT& buffer, SizeT fileSize);
    private:
        virtual std::unique_ptr<Asset::AudioAssetData> CreateAudioDataImpl(
            ExtractedFile&& extracted, const File::Name& name) = 0;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Audio::AudioManager, BinaryArchive> :
        public TableScribe<::Atmos::Audio::AudioManager, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        protected:
            void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
        };
    };
}