#include <XAudio2.h>
#include <vorbis/vorbisfile.h>

#include "AudioManager.h"
#include "UnrecognizedAudioFormat.h"

#include "Buffer.h"

namespace Atmos::Audio
{
    struct OggFile
    {
        const char* currentPosition;
        const char* dataSource;
        size_t fileSize;
        OggFile(const char* dataSource, size_t fileSize) :
            currentPosition(dataSource), dataSource(dataSource), fileSize(fileSize)
        {}
    };

    size_t ReadOggFile(void* ptr, size_t size, size_t nmemb, void* dataSource)
    {
        auto oggFile = reinterpret_cast<OggFile*>(dataSource);
        auto readCount = size * nmemb;

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

    std::unique_ptr<Asset::AudioAssetData> AudioManager::CreateAudioData(
        const Buffer& buffer, const Name& name)
    {
        const auto formattedBuffer = FormatBuffer(buffer, name);
        return CreateAudioDataImpl(formattedBuffer, name);
    }

    AudioManager::AudioManager() = default;

    auto AudioManager::FormatBuffer(const Buffer& buffer, const Name& name) -> FormattedBuffer
    {
        switch (FileTypeOf(buffer))
        {
        case FileType::Wav:
            return FormatBufferWAV(buffer);
        case FileType::Ogg:
            return FormatBufferOGG(buffer);
        default:
            throw UnrecognizedAudioFormat(name);
        }
    }

    auto AudioManager::FormatBufferWAV(const Buffer& buffer) -> FormattedBuffer
    {
        Format format{};

        // Get WAV format
        const unsigned int audioFormatPosition = 20;
        const unsigned int durationPosition = 40;

        auto currentPosition = audioFormatPosition;

        // Audio format
        currentPosition += ReadFromBuffer(buffer.data(), format.formatTag, currentPosition);
        // Number of channels
        currentPosition += ReadFromBuffer(buffer.data(), format.channels, currentPosition);
        // Sample rate
        currentPosition += ReadFromBuffer(buffer.data(), format.samplesPerSec, currentPosition);
        // Byte rate
        currentPosition += ReadFromBuffer(buffer.data(), format.avgBytesPerSec, currentPosition);
        // Block align
        currentPosition += ReadFromBuffer(buffer.data(), format.blockAlign, currentPosition);
        // Bits per sample
        currentPosition += ReadFromBuffer(buffer.data(), format.bitsPerSample, currentPosition);

        currentPosition = durationPosition;

        size_t retSize;
        currentPosition += ReadFromBuffer(buffer.data(), retSize, currentPosition);

        // Fill sound data
        auto newBuffer = Buffer(retSize);
        currentPosition += CopyBuffer(buffer.data(), newBuffer.data(), retSize, currentPosition);

        return FormattedBuffer{ std::move(newBuffer), format};
    }

    auto AudioManager::FormatBufferOGG(const Buffer& buffer) -> FormattedBuffer
    {
        Format format{};

        OggFile oggFile(buffer.data(), buffer.size());

        OggVorbis_File vorbisFile;
        ov_open_callbacks(&oggFile, &vorbisFile, nullptr, -1, CreateOggCallbacks());

        auto byteSize = static_cast<unsigned int>(ov_pcm_total(&vorbisFile, -1) * 4);
        auto totalBuffer = Buffer(byteSize);

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

        return FormattedBuffer{ totalBuffer, format };
    }

    AudioManager::FileType AudioManager::FileTypeOf(const Buffer& buffer)
    {
        // Test WAV
        {
            // Check magic string
            // 82 73 70 70
            // RIFF
            const char size = 4;
            const char magicString[size]{ 82, 73, 70, 70 };
            if (CompareBuffers(buffer.data(), &magicString, size))
                return FileType::Wav;
        }

        // Test OGG
        {
            OggFile oggFile(buffer.data(), buffer.size());

            OggVorbis_File vorbisFile;
            auto result = ov_test_callbacks(&oggFile, &vorbisFile, nullptr, 0, CreateOggCallbacks());
            ov_clear(&vorbisFile);
            if (result >= 0)
                return FileType::Ogg;
        }

        return FileType::Unknown;
    }
}