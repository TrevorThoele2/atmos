#include <XAudio2.h>
#include <vorbis/vorbisfile.h>

#include "AudioManager.h"

#include "Buffer.h"
#include "SimpleFile.h"

namespace Atmos::Audio
{
    struct OggFile
    {
        char* currentPosition;
        char* dataSource;
        AudioManager::SizeT fileSize;
        OggFile(char* dataSource, AudioManager::SizeT fileSize) :
            currentPosition(dataSource), dataSource(dataSource), fileSize(fileSize)
        {}
    };

    size_t ReadOggFile(void* ptr, size_t size, size_t nmemb, void* dataSource)
    {
        auto oggFile = reinterpret_cast<OggFile*>(dataSource);
        size_t readCount = size * nmemb;

        // If going to read past the frameEndTime
        if (oggFile->currentPosition + readCount > oggFile->dataSource + oggFile->fileSize)
            readCount = oggFile->dataSource + oggFile->fileSize - oggFile->currentPosition;

        memcpy(ptr, oggFile->currentPosition, readCount);
        oggFile->currentPosition += readCount;
        return readCount;
    }

    int SeekOggFile(void* dataSource, ogg_int64_t offset, int type)
    {
        auto oggFile = reinterpret_cast<OggFile*>(dataSource);

        switch (type)
        {
        case SEEK_CUR:
            oggFile->currentPosition += offset;
            break;
        case SEEK_END:
            oggFile->currentPosition = oggFile->dataSource + (oggFile->fileSize - offset);
            break;
        case SEEK_SET:
            oggFile->currentPosition = oggFile->dataSource + offset;
            break;
        default:
            return -1;
        }

        if (oggFile->currentPosition < oggFile->dataSource)
        {
            oggFile->currentPosition = oggFile->dataSource;
            return -1;
        }

        if (oggFile->currentPosition > oggFile->dataSource + oggFile->fileSize)
        {
            oggFile->currentPosition = oggFile->dataSource + oggFile->fileSize;
            return -1;
        }

        return 0;
    }

    int CloseOggFile(void*)
    {
        return 0;
    }

    long TellOggFile(void* dataSource)
    {
        const auto oggFile = reinterpret_cast<OggFile*>(dataSource);
        return oggFile->currentPosition - oggFile->dataSource;
    }

    ov_callbacks CreateOggCallbacks()
    {
        ov_callbacks callbacks;
        callbacks.read_func = &ReadOggFile;
        callbacks.seek_func = &SeekOggFile;
        callbacks.close_func = &CloseOggFile;
        callbacks.tell_func = &TellOggFile;
        return callbacks;
    }

    AudioManager::~AudioManager() = default;

    std::unique_ptr<Asset::AudioAssetData> AudioManager::CreateAudioData(const File::Path& path)
    {
        SimpleInFile file(path);
        const auto fileSize = static_cast<BufferT::SizeT>(file.GetFileSize());

        auto buffer = BufferT(fileSize);

        file.FillBuffer(buffer.GetBytes(), fileSize);

        const auto typeOfBuffer = FileTypeOf(buffer, fileSize);
        auto createdData = ExtractFile(typeOfBuffer, std::move(buffer), fileSize);
        auto asset = CreateAudioDataImpl(std::move(createdData), path.GetFileName());

        return asset;
    }

    std::unique_ptr<Asset::AudioAssetData> AudioManager::CreateAudioData(
        void* buffer, SizeT fileSize, const File::Name& name)
    {
        BufferT madeBuffer(buffer, fileSize);
        auto data = ExtractFile(FileTypeOf(madeBuffer, fileSize), std::move(madeBuffer), fileSize);
        return CreateAudioDataImpl(std::move(data), name);
    }

    bool AudioManager::CanMake(const File::Path& path)
    {
        SimpleInFile file(path);
        const auto fileSize = static_cast<SizeT>(file.GetFileSize());

        const auto buffer = BufferT(fileSize);
        file.FillBuffer(buffer.GetBytes(), fileSize);

        return FileTypeOf(buffer, fileSize) != FileType::None;
    }

    bool AudioManager::CanMake(void* buffer, SizeT fileSize)
    {
        return FileTypeOf(BufferT(buffer, fileSize), fileSize) != FileType::None;
    }

    AudioManager::AudioManager() = default;

    AudioManager::ExtractedFile AudioManager::ExtractFile(FileType fileType, BufferT&& buffer, SizeT fileSize)
    {
        if (fileType == FileType::Wav)
            return ExtractFileWAV(std::move(buffer), fileSize);
        else
            return ExtractFileOGG(std::move(buffer), fileSize);
    }

    AudioManager::ExtractedFile AudioManager::ExtractFileWAV(BufferT&& buffer, SizeT fileSize)
    {
        Format format{};

        // Get WAV format
        const unsigned int AUDIOFORMAT_POSITION = 20;
        const unsigned int DURATION_POSITION = 40;

        size_t currentPosition = AUDIOFORMAT_POSITION;

        // Audio format
        currentPosition += ReadFromBuffer(buffer.GetBytes(), format.formatTag, currentPosition);
        // Number of channels
        currentPosition += ReadFromBuffer(buffer.GetBytes(), format.channels, currentPosition);
        // Sample rate
        currentPosition += ReadFromBuffer(buffer.GetBytes(), format.samplesPerSec, currentPosition);
        // Byte rate
        currentPosition += ReadFromBuffer(buffer.GetBytes(), format.avgBytesPerSec, currentPosition);
        // Block align
        currentPosition += ReadFromBuffer(buffer.GetBytes(), format.blockAlign, currentPosition);
        // Bits per sample
        currentPosition += ReadFromBuffer(buffer.GetBytes(), format.bitsPerSample, currentPosition);

        currentPosition = DURATION_POSITION;

        BufferT::SizeT retSize;
        currentPosition += ReadFromBuffer(buffer.GetBytes(), retSize, currentPosition);

        // Fill sound data
        auto newBuffer = BufferT(retSize);
        currentPosition += CopyBuffer(buffer.GetBytes(), newBuffer.GetBytes(), retSize, currentPosition);

        return ExtractedFile(std::move(newBuffer), format);
    }

    AudioManager::ExtractedFile AudioManager::ExtractFileOGG(BufferT&& buffer, SizeT fileSize)
    {
        Format format{};

        OggFile oggFile(buffer.GetBytes(), fileSize);

        OggVorbis_File vorbisFile;
        ov_open_callbacks(&oggFile, &vorbisFile, nullptr, -1, CreateOggCallbacks());

        auto byteSize = static_cast<unsigned int>(ov_pcm_total(&vorbisFile, -1) * 4);
        auto totalBuffer = new char[byteSize];

        // Create buffer
        const size_t size = 4096;
        char bufferTemp[size];
        memset(bufferTemp, 0, size);

        // Read the entirety of the sound from the ogg file
        size_t totalRead = 0;
        int returned = ov_read(&vorbisFile, bufferTemp, size, 0, 2, 1, nullptr);
        totalRead += returned;
        while (returned != 0)
        {
            returned = ov_read(&vorbisFile, bufferTemp, size, 0, 2, 1, nullptr);
            memcpy(&totalBuffer[totalRead], bufferTemp, returned);
            totalRead += returned;
        }

        auto info = ov_info(&vorbisFile, -1);

        format.formatTag = WAVE_FORMAT_PCM;
        format.channels = info->channels;
        format.samplesPerSec = info->rate;
        format.bitsPerSample = format.channels * 8;
        format.blockAlign = (format.channels * format.bitsPerSample) / 8;
        format.avgBytesPerSec = format.samplesPerSec * format.blockAlign;

        ov_clear(&vorbisFile);

        return ExtractedFile(BufferT(totalBuffer, byteSize), format);
    }

    AudioManager::FileType AudioManager::FileTypeOf(const BufferT& buffer, SizeT fileSize)
    {
        // Test WAV
        {
            // Check magic string
            // 82 73 70 70
            // RIFF
            const char size = 4;
            const char magicString[size]{ 82, 73, 70, 70 };
            if (CompareBuffers(buffer.GetBytes(), &magicString, size))
                return FileType::Wav;
        }

        // Test OGG
        {
            OggFile oggFile(buffer.GetBytes(), fileSize);

            OggVorbis_File vorbisFile;
            auto result = ov_test_callbacks(&oggFile, &vorbisFile, nullptr, 0, CreateOggCallbacks());
            ov_clear(&vorbisFile);
            if (result >= 0)
                return FileType::Ogg;
        }

        return FileType::None;
    }
}

namespace Arca
{
    const TypeName Traits<Atmos::Audio::AudioManager>::typeName = "AudioManager";
}