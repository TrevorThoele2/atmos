#include "ImageAssetCurator.h"

#include "LoadAssetError.h"

namespace Atmos::Asset
{
	ImageCurator::ImageCurator(Init init) : Curator(init)
	{}

	Resource::Loaded<Resource::Image> ImageCurator::Handle(const Resource::LoadFromFile<Resource::Image>& command)
    {
        const auto filePath = command.filePath.string();
		const auto format = FIFFor(filePath);
        if (format == FIF_UNKNOWN)
            throw LoadError("Loading an image asset has encountered an error.", { {"FilePath", filePath } });
        const auto loadedBitmap = FreeImage_Load(format, filePath.c_str());
		try
		{
			if (!FreeImage_HasPixels(loadedBitmap))
				throw LoadError("Attempted load of an image asset without pixels.", { {"FilePath", filePath } });

			return ProcessBitmap(loadedBitmap, format);
		}
		catch (...)
		{
			FreeImage_Unload(loadedBitmap);
			throw;
		}
    }

	Resource::Loaded<Resource::Image> ImageCurator::Handle(const Resource::LoadFromMemory<Resource::Image>& command)
	{
	    auto memory = command.memory;
		const auto size = memory.size();

	    const auto freeImageMemory = FreeImage_OpenMemory(memory.data(), size);
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
		catch(...)
		{
			FreeImage_CloseMemory(freeImageMemory);
			throw;
		}
	}

	Resource::Loaded<Resource::Image> ImageCurator::ProcessBitmap(FIBITMAP* loadedBitmap, FREE_IMAGE_FORMAT format)
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
			DataBuffer buffer;
			buffer.insert(buffer.begin(), bits, bits + byteSize);

			FreeImage_Unload(bitmap);

			return Resource::Loaded<Resource::Image>
		    {
				std::move(buffer),
				*TypeFromFIF(format),
				ImageSize{ ImageSize::Dimension(width), ImageSize::Dimension(height) }
		    };
		}
		catch (...)
		{
			FreeImage_Unload(bitmap);
			throw;
		}
	}

    FREE_IMAGE_FORMAT ImageCurator::FIFFor(const String& filePath)
	{
		const auto format = FreeImage_GetFileType(filePath.c_str());
		return format != FIF_UNKNOWN
		    ? format
			: FreeImage_GetFIFFromFilename(filePath.c_str());
	}

    FREE_IMAGE_FORMAT ImageCurator::FIFFor(FIMEMORY& memory, int size)
	{
		return FreeImage_GetFileTypeFromMemory(&memory, size);
	}

    std::optional<ImageType> ImageCurator::TypeFromFIF(FREE_IMAGE_FORMAT format)
    {
        switch (format)
        {
		case FIF_BMP:
			return ImageType::Bmp;
		case FIF_ICO:
			return ImageType::Ico;
		case FIF_JPEG:
			return ImageType::Jpeg;
		case FIF_JNG:
			return ImageType::Jng;
		case FIF_KOALA:
			return ImageType::Koala;
		case FIF_LBM:
			return ImageType::Lbm;
		case FIF_MNG:
			return ImageType::Mng;
		case FIF_PBM:
			return ImageType::Pbm;
		case FIF_PBMRAW:
			return ImageType::Pbmraw;
		case FIF_PCD:
			return ImageType::Pcd;
		case FIF_PCX:
			return ImageType::Pcx;
		case FIF_PGM:
			return ImageType::Pgm;
		case FIF_PGMRAW:
			return ImageType::Pgmraw;
		case FIF_PNG:
			return ImageType::Png;
		case FIF_PPM:
			return ImageType::Ppm;
		case FIF_PPMRAW:
			return ImageType::Ppmraw;
		case FIF_RAS:
			return ImageType::Ras;
		case FIF_TARGA:
			return ImageType::Targa;
		case FIF_TIFF:
			return ImageType::Tiff;
		case FIF_WBMP:
			return ImageType::Wbmp;
		case FIF_PSD:
			return ImageType::Psd;
		case FIF_CUT:
			return ImageType::Cut;
		case FIF_XBM:
			return ImageType::Xbm;
		case FIF_XPM:
			return ImageType::Xpm;
		case FIF_DDS:
			return ImageType::Dds;
		case FIF_GIF:
			return ImageType::Gif;
		case FIF_HDR:
			return ImageType::Hdr;
		case FIF_FAXG3:
			return ImageType::Faxg3;
		case FIF_SGI:
			return ImageType::Sgi;
		case FIF_EXR:
			return ImageType::Exr;
		case FIF_J2K:
			return ImageType::J2k;
		case FIF_JP2:
			return ImageType::Jp2;
		case FIF_PFM:
			return ImageType::Pfm;
		case FIF_PICT:
			return ImageType::Pict;
		case FIF_RAW:
			return ImageType::Raw;
		case FIF_WEBP:
			return ImageType::Webp;
		case FIF_JXR:
			return ImageType::Jxr;
		default:
			return {};
        }
    }
}