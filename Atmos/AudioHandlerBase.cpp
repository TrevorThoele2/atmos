
#include <XAudio2.h>
#include <vorbis\vorbisfile.h>
#include "AudioHandlerBase.h"

#include "Buffer.h"
#include "SimpleFile.h"

namespace Atmos
{
    struct OggFile
    {
        char* currentPosition;
        char* dataSource;
        AudioHandlerBase::SizeT fileSize;
        OggFile(char* dataSource, AudioHandlerBase::SizeT fileSize) :
            dataSource(std::move(dataSource)), currentPosition(dataSource), fileSize(fileSize)
        {}
    };

    size_t ReadOggFile(void* ptr, size_t size, size_t nmemb, void* dataSource)
    {
        OggFile* oggFile = reinterpret_cast<OggFile*>(dataSource);
        size_t readCount = size * nmemb;

        // If going to read past the end
        if (oggFile->currentPosition + readCount > oggFile->dataSource + oggFile->fileSize)
            readCount = oggFile->dataSource + oggFile->fileSize - oggFile->currentPosition;

        memcpy(ptr, oggFile->currentPosition, readCount);
        oggFile->currentPosition += readCount;
        return readCount;
    }

    int SeekOggFile(void* dataSource, ogg_int64_t offset, int type)
    {
        OggFile* oggFile = reinterpret_cast<OggFile*>(dataSource);

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

    int CloseOggFile(void* datasource)
    {
        return 0;
    }

    long TellOggFile(void* dataSource)
    {
        OggFile* oggFile = reinterpret_cast<OggFile*>(dataSource);
        return (oggFile->currentPosition - oggFile->dataSource);
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

    std::unique_ptr<AudioAssetData> AudioHandlerBase::CreateAudioData(const FilePath& path)
    {
        SimpleInFile file(path);
        BufferT::SizeT fileSize = static_cast<BufferT::SizeT>(file.GetFileSize());

        auto buffer = BufferT(fileSize);

        file.FillBuffer(buffer.GetBytes(), fileSize);

        auto typeOfBuffer = FileTypeOf(buffer, fileSize);
        auto& createdData = ExtractFile(typeOfBuffer, std::move(buffer), fileSize);
        auto& asset = CreateAudioDataImpl(std::move(createdData), path.GetFileName());

        return std::move(asset);
    }

    std::unique_ptr<AudioAssetData> AudioHandlerBase::CreateAudioData(void* buffer, SizeT fileSize, const FileName& name)
    {
        BufferT madeBuffer(buffer, fileSize);
        auto& data = ExtractFile(FileTypeOf(madeBuffer, fileSize), std::move(madeBuffer), fileSize);
        return CreateAudioDataImpl(std::move(data), name);
    }

    bool AudioHandlerBase::CanMake(const FilePath& path)
    {
        SimpleInFile file(path);
        auto fileSize = static_cast<SizeT>(file.GetFileSize());

        auto buffer = BufferT(fileSize);
        file.FillBuffer(buffer.GetBytes(), fileSize);

        bool ret = FileTypeOf(buffer, fileSize) != FileType::NONE;
        return ret;
    }

    bool AudioHandlerBase::CanMake(void* buffer, SizeT fileSize)
    {
        return FileTypeOf(BufferT(buffer, fileSize), fileSize) != FileType::NONE;
    }

    AudioHandlerBase::ExtractedFile AudioHandlerBase::ExtractFile(FileType fileType, BufferT&& buffer, SizeT fileSize)
    {
        if (fileType == FileType::WAV)
            return ExtractFileWAV(std::move(buffer), fileSize);
        else
            return ExtractFileOGG(std::move(buffer), fileSize);
    }

    AudioHandlerBase::ExtractedFile AudioHandlerBase::ExtractFileWAV(BufferT&& buffer, SizeT fileSize)
    {
        Format format;

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

        return ExtractedFile(std::move(newBuffer), std::move(format));
    }

    AudioHandlerBase::ExtractedFile AudioHandlerBase::ExtractFileOGG(BufferT&& buffer, SizeT fileSize)
    {
        Format format;

        OggFile oggFile(buffer.GetBytes(), fileSize);

        OggVorbis_File og_file;
        ov_open_callbacks(&oggFile, &og_file, nullptr, -1, CreateOggCallbacks());

        auto byteSize = static_cast<unsigned int>(ov_pcm_total(&og_file, -1) * 4);
        auto totalBuffer = new char[byteSize];

        // Create buffer
        const size_t size = 4096;
        char bufferTemp[size];
        memset(bufferTemp, 0, size);

        // Read the entirety of the sound from the ogg file
        size_t totalRead = 0;
        int returned = ov_read(&og_file, bufferTemp, size, 0, 2, 1, nullptr);
        totalRead += returned;
        while (returned != 0)
        {
            returned = ov_read(&og_file, bufferTemp, size, 0, 2, 1, nullptr);
            memcpy(&totalBuffer[totalRead], bufferTemp, returned);
            totalRead += returned;
        }

        auto info = ov_info(&og_file, -1);

        format.formatTag = WAVE_FORMAT_PCM;
        format.channels = info->channels;
        format.samplesPerSec = info->rate;
        format.bitsPerSample = format.channels * 8;
        format.blockAlign = (format.channels * format.bitsPerSample) / 8;
        format.avgBytesPerSec = format.samplesPerSec * format.blockAlign;

        ov_clear(&og_file);

        return ExtractedFile(BufferT(totalBuffer, byteSize), std::move(format));
    }

    AudioHandlerBase::FileType AudioHandlerBase::FileTypeOf(const BufferT& buffer, SizeT fileSize)
    {
        // Test WAV
        {
            // Check magic string
            // 82 73 70 70
            // RIFF
            const char size = 4;
            const char magicString[size]{ 82, 73, 70, 70 };
            if (CompareBuffers(buffer.GetBytes(), &magicString, size))
                return FileType::WAV;
        }

        // Test OGG
        {
            OggFile oggFile(buffer.GetBytes(), fileSize);

            OggVorbis_File og_file;
            auto result = ov_test_callbacks(&oggFile, &og_file, nullptr, 0, CreateOggCallbacks());
            ov_clear(&og_file);
            if (result >= 0)
                return FileType::OGG;
        }

        return FileType::NONE;
    }
}