#include "RealAssetResourceManager.h"

#include "LoadAssetError.h"

#include "String.h"

namespace Atmos::Asset::Resource
{
	RealManager::RealManager(Logging::Logger& logger) : Manager(logger, "Real"), freeImageManager(logger)
	{}

	LoadedData<Image> RealManager::LoadImageData(const File::Path& filePath)
	{
		return freeImageManager.Load(filePath);
	}

	LoadedData<Image> RealManager::LoadImageData(const Buffer& memory)
	{
		return freeImageManager.Load(memory);
	}

	LoadedData<Audio> RealManager::LoadAudioData(const File::Path& filePath)
	{
		return audioManager.Load(filePath);
	}

	LoadedData<Audio> RealManager::LoadAudioData(const Buffer& memory)
	{
		return audioManager.Load(memory);
	}

	RealManager::ImageManager::ImageManager(Logging::Logger& logger)
	{
		_CrtSetDbgFlag(0);
		FreeImage_Initialise();
		FreeImage_SetOutputMessage(ErrorHandler);

	    ImageManager::logger = &logger;
	}

	RealManager::ImageManager::~ImageManager()
	{
		FreeImage_DeInitialise();
	}

	Logging::Logger* RealManager::ImageManager::logger = nullptr;

	void RealManager::ImageManager::ErrorHandler(FREE_IMAGE_FORMAT format, const char* message)
	{
		const auto retrievedFormatMessage = FreeImage_GetFormatFromFIF(format);
		const String formatMessage = retrievedFormatMessage
			? retrievedFormatMessage
			: "Unknown";

		logger->Log(
			message,
			Logging::Severity::Error,
			Logging::Details{ { "Format", formatMessage } });
	}

	LoadedData<Image> RealManager::ImageManager::Load(const File::Path& filePath)
	{
		const auto useFilePath = filePath.string();
		const auto format = FIFFor(useFilePath);
		if (format == FIF_UNKNOWN)
			throw LoadError("Loading an image asset has encountered an error.", { {"FilePath", useFilePath } });
		const auto loadedBitmap = FreeImage_Load(format, useFilePath.c_str());
		try
		{
			if (!FreeImage_HasPixels(loadedBitmap))
				throw LoadError("Attempted load of an image asset without pixels.", { {"FilePath", useFilePath } });

			return ProcessBitmap(loadedBitmap, format);
		}
		catch (...)
		{
			FreeImage_Unload(loadedBitmap);
			throw;
		}
	}

	LoadedData<Image> RealManager::ImageManager::Load(const Buffer& memory)
	{
		auto useMemory = ToUnsignedBuffer(memory);
		const auto size = useMemory.size();

		const auto freeImageMemory = FreeImage_OpenMemory(useMemory.data(), size);
		try
		{
			const auto format = FIFFor(*freeImageMemory, 0);
			if (format == FIF_UNKNOWN)
				throw LoadError("Loading an image asset has encountered an error.");

			const auto loadedBitmap = FreeImage_LoadFromMemory(format, freeImageMemory);
			try
			{
				if (!FreeImage_HasPixels(loadedBitmap))
					throw LoadError("Attempted load of an image asset without pixels.");

				return ProcessBitmap(loadedBitmap, format);
			}
			catch (...)
			{
				FreeImage_Unload(loadedBitmap);
				throw;
			}
		}
		catch (...)
		{
			FreeImage_CloseMemory(freeImageMemory);
			throw;
		}
	}

	LoadedData<Image> RealManager::ImageManager::ProcessBitmap(
		FIBITMAP* loadedBitmap, FREE_IMAGE_FORMAT format)
	{
		const auto bitmap = FreeImage_ConvertTo32Bits(loadedBitmap);

		try
		{
			FreeImage_Unload(loadedBitmap);

			FreeImage_FlipVertical(bitmap);

			SwapRedBlue32(bitmap);

			const auto width = FreeImage_GetWidth(bitmap);
			const auto height = FreeImage_GetHeight(bitmap);
			const auto bitsPerPixel = FreeImage_GetBPP(bitmap);
			const auto bits = FreeImage_GetBits(bitmap);

			const auto byteSize = bitsPerPixel / CHAR_BIT * width * height;
			Buffer buffer;
			buffer.insert(buffer.begin(), bits, bits + byteSize);

			FreeImage_Unload(bitmap);

			return LoadedData<Image>
			{
				std::move(buffer),
					* TypeFromFIF(format),
					ImageSize{ ImageSize::Dimension(width), ImageSize::Dimension(height) }
			};
		}
		catch (...)
		{
			FreeImage_Unload(bitmap);
			throw;
		}
	}

	FREE_IMAGE_FORMAT RealManager::ImageManager::FIFFor(const String& filePath)
	{
		const auto format = FreeImage_GetFileType(filePath.c_str());
		return format != FIF_UNKNOWN
			? format
			: FreeImage_GetFIFFromFilename(filePath.c_str());
	}

	FREE_IMAGE_FORMAT RealManager::ImageManager::FIFFor(FIMEMORY& memory, int size)
	{
		return FreeImage_GetFileTypeFromMemory(&memory, size);
	}

	std::optional<ImageFileType> RealManager::ImageManager::TypeFromFIF(FREE_IMAGE_FORMAT format)
	{
		switch (format)
		{
		case FIF_BMP:
			return ImageFileType::Bmp;
		case FIF_ICO:
			return ImageFileType::Ico;
		case FIF_JPEG:
			return ImageFileType::Jpeg;
		case FIF_JNG:
			return ImageFileType::Jng;
		case FIF_KOALA:
			return ImageFileType::Koala;
		case FIF_LBM:
			return ImageFileType::Lbm;
		case FIF_MNG:
			return ImageFileType::Mng;
		case FIF_PBM:
			return ImageFileType::Pbm;
		case FIF_PBMRAW:
			return ImageFileType::Pbmraw;
		case FIF_PCD:
			return ImageFileType::Pcd;
		case FIF_PCX:
			return ImageFileType::Pcx;
		case FIF_PGM:
			return ImageFileType::Pgm;
		case FIF_PGMRAW:
			return ImageFileType::Pgmraw;
		case FIF_PNG:
			return ImageFileType::Png;
		case FIF_PPM:
			return ImageFileType::Ppm;
		case FIF_PPMRAW:
			return ImageFileType::Ppmraw;
		case FIF_RAS:
			return ImageFileType::Ras;
		case FIF_TARGA:
			return ImageFileType::Targa;
		case FIF_TIFF:
			return ImageFileType::Tiff;
		case FIF_WBMP:
			return ImageFileType::Wbmp;
		case FIF_PSD:
			return ImageFileType::Psd;
		case FIF_CUT:
			return ImageFileType::Cut;
		case FIF_XBM:
			return ImageFileType::Xbm;
		case FIF_XPM:
			return ImageFileType::Xpm;
		case FIF_DDS:
			return ImageFileType::Dds;
		case FIF_GIF:
			return ImageFileType::Gif;
		case FIF_HDR:
			return ImageFileType::Hdr;
		case FIF_FAXG3:
			return ImageFileType::Faxg3;
		case FIF_SGI:
			return ImageFileType::Sgi;
		case FIF_EXR:
			return ImageFileType::Exr;
		case FIF_J2K:
			return ImageFileType::J2k;
		case FIF_JP2:
			return ImageFileType::Jp2;
		case FIF_PFM:
			return ImageFileType::Pfm;
		case FIF_PICT:
			return ImageFileType::Pict;
		case FIF_RAW:
			return ImageFileType::Raw;
		case FIF_WEBP:
			return ImageFileType::Webp;
		case FIF_JXR:
			return ImageFileType::Jxr;
		default:
			return {};
		}
	}

	LoadedData<Audio> RealManager::AudioManager::Load(const File::Path& filePath)
	{
		auto inputArchive = Inscription::Archive::InputBinary(filePath);
		const auto size = inputArchive.Size();

		Inscription::Buffer loadedBuffer;
		loadedBuffer.value.resize(static_cast<size_t>(size));
		inputArchive.Read(loadedBuffer);

		return Load(Buffer(loadedBuffer.value.begin(), loadedBuffer.value.end()));
	}

	LoadedData<Audio> RealManager::AudioManager::Load(const Buffer& memory)
	{
		return LoadedData<Audio>{memory};
	}
}