
#include <XAudio2.h>
#include <vorbis\vorbisfile.h>
#include "AudioHandlerBase.h"

#include "Buffer.h"
#include "SimpleFile.h"

namespace Atmos
{
    struct OggFile
    {
        char *currentPosition;
        char *dataSource;
        AudioHandlerBase::SizeT fileSize;
        OggFile(char *dataSource, AudioHandlerBase::SizeT fileSize) : dataSource(dataSource), currentPosition(dataSource), fileSize(fileSize)
        {}
    };

    size_t ReadOggFile(void *ptr, size_t size, size_t nmemb, void *dataSource)
    {
        OggFile *oggFile = reinterpret_cast<OggFile*>(dataSource);
        size_t readCount = size * nmemb;

        // If going to read past the end
        if (oggFile->currentPosition + readCount > oggFile->dataSource + oggFile->fileSize)
            readCount = oggFile->dataSource + oggFile->fileSize - oggFile->currentPosition;

        memcpy(ptr, oggFile->currentPosition, readCount);
        oggFile->currentPosition += readCount;
        return readCount;
    }

    int SeekOggFile(void *dataSource, ogg_int64_t offset, int type)
    {
        OggFile *oggFile = reinterpret_cast<OggFile*>(dataSource);

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

    int CloseOggFile(void *datasource)
    {
        return 0;
    }

    long TellOggFile(void *dataSource)
    {
        OggFile *oggFile = reinterpret_cast<OggFile*>(dataSource);
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

    AudioHandlerBase::Data AudioHandlerBase::CreateData(Type type, void *buffer, SizeT fileSize)
    {
        if (type == Type::WAV)
            return CreateDataWAV(buffer, fileSize);
        else // OGG
            return CreateDataOGG(buffer, fileSize);
    }

    AudioHandlerBase::Data AudioHandlerBase::CreateDataWAV(void *buffer, SizeT fileSize)
    {
        Format format;

        // Get WAV format
        const unsigned int AUDIOFORMAT_POSITION = 20;
        const unsigned int DURATION_POSITION = 40;

        size_t currentPosition = AUDIOFORMAT_POSITION;

        // Audio format
        currentPosition += ReadFromBuffer(buffer, format.formatTag, currentPosition);
        // Number of channels
        currentPosition += ReadFromBuffer(buffer, format.channels, currentPosition);
        // Sample rate
        currentPosition += ReadFromBuffer(buffer, format.samplesPerSec, currentPosition);
        // Byte rate
        currentPosition += ReadFromBuffer(buffer, format.avgBytesPerSec, currentPosition);
        // Block align
        currentPosition += ReadFromBuffer(buffer, format.blockAlign, currentPosition);
        // Bits per sample
        currentPosition += ReadFromBuffer(buffer, format.bitsPerSample, currentPosition);

        currentPosition = DURATION_POSITION;

        BufferT::SizeT retSize;
        currentPosition += ReadFromBuffer(buffer, retSize, currentPosition);

        // Fill sound data
        auto temp = new char[retSize];
        currentPosition += ReadBufferFromBuffer(buffer, temp, retSize, currentPosition);

        return Data(BufferT(temp, retSize), std::move(format));
    }

    AudioHandlerBase::Data AudioHandlerBase::CreateDataOGG(void *buffer, SizeT fileSize)
    {
        Format format;

        OggFile oggFile(reinterpret_cast<char*>(buffer), fileSize);

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

        return Data(BufferT(totalBuffer, byteSize), std::move(format));
    }

    AudioHandlerBase::Type AudioHandlerBase::GetType(void *buffer, SizeT fileSize)
    {
        // Test WAV
        {
            // Check magic string
            // 82 73 70 70
            // RIFF
            const char size = 4;
            const char magicString[size]{ 82, 73, 70, 70 };
            if (CompareBuffers(buffer, &magicString, size))
                return Type::WAV;
        }

        // Test OGG
        {
            OggFile oggFile(reinterpret_cast<char*>(buffer), fileSize);

            OggVorbis_File og_file;
            auto result = ov_test_callbacks(&oggFile, &og_file, nullptr, 0, CreateOggCallbacks());
            if (result >= 0)
                return Type::OGG;
        }

        return Type::NONE;
    }

    AudioAsset AudioHandlerBase::CreateAudio(const FilePath &path)
    {
        SimpleInFile file(path);
        BufferT::SizeT fileSize = static_cast<BufferT::SizeT>(file.GetFileSize());
        char *buffer = new char[static_cast<size_t>(fileSize)];
        file.FillBuffer(buffer, fileSize);

        auto &ret = CreateAudioImpl(CreateData(GetType(buffer, fileSize), buffer, fileSize), path.GetFileName());
        delete[] buffer;
        return std::move(ret);
    }

    AudioAsset AudioHandlerBase::CreateAudio(void *buffer, SizeT fileSize, const FileName &name)
    {
        return CreateAudioImpl(CreateData(GetType(buffer, fileSize), buffer, fileSize), name);
    }

    bool AudioHandlerBase::CanMake(const FilePath &path)
    {
        SimpleInFile file(path);
        auto fileSize = file.GetFileSize();
        char *buffer = new char[static_cast<size_t>(fileSize)];
        file.FillBuffer(buffer, fileSize);

        bool ret = GetType(buffer, static_cast<SizeT>(fileSize)) != Type::NONE;
        delete[] buffer;
        return ret;
    }

    bool AudioHandlerBase::CanMake(void *buffer, SizeT fileSize)
    {
        return GetType(buffer, fileSize) != Type::NONE;
    }
}