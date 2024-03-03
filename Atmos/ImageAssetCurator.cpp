#include "ImageAssetCurator.h"

#include "LoadAssetError.h"

namespace Atmos::Asset
{
	ImageAssetCurator::ImageAssetCurator(Init init) : AssetCurator(init)
	{}

    LoadedImageAsset ImageAssetCurator::Handle(const LoadImageAsset& command)
    {
        const auto filePath = command.filePath.string();
        auto format = FreeImage_GetFileType(filePath.c_str());
        if (format == FIF_UNKNOWN)
            format = FreeImage_GetFIFFromFilename(filePath.c_str());
        if (format == FIF_UNKNOWN)
            throw LoadAssetError("Loading an image asset has encountered an error.", { {"FilePath", filePath } });
        const auto bitmap = FreeImage_Load(format, filePath.c_str());
		try
		{
			if (!FreeImage_HasPixels(bitmap))
				throw LoadAssetError("Attempted load of an image asset without pixels.", { {"FilePath", filePath } });
			const auto width = FreeImage_GetWidth(bitmap);
			const auto height = FreeImage_GetHeight(bitmap);
			const auto bitsPerPixel = FreeImage_GetBPP(bitmap);
			const auto bits = FreeImage_GetBits(bitmap);

			const auto byteSize = bitsPerPixel / 8 * width * height;
			Buffer buffer;
			buffer.insert(buffer.begin(), bits, bits + byteSize);

			FreeImage_Unload(bitmap);

			return LoadedImageAsset{
			    std::move(buffer),
			    *TypeFromFIF(format),
			    Size2D{ Size2D::Value(width), Size2D::Value(height) } };
		}
		catch (...)
		{
			FreeImage_Unload(bitmap);
			throw;
		}
    }

    std::optional<ImageAssetType> ImageAssetCurator::TypeFromFIF(FREE_IMAGE_FORMAT format)
    {
        switch (format)
        {
		case FIF_BMP:
			return ImageAssetType::Bmp;
		case FIF_ICO:
			return ImageAssetType::Ico;
		case FIF_JPEG:
			return ImageAssetType::Jpeg;
		case FIF_JNG:
			return ImageAssetType::Jng;
		case FIF_KOALA:
			return ImageAssetType::Koala;
		case FIF_LBM:
			return ImageAssetType::Lbm;
		case FIF_MNG:
			return ImageAssetType::Mng;
		case FIF_PBM:
			return ImageAssetType::Pbm;
		case FIF_PBMRAW:
			return ImageAssetType::Pbmraw;
		case FIF_PCD:
			return ImageAssetType::Pcd;
		case FIF_PCX:
			return ImageAssetType::Pcx;
		case FIF_PGM:
			return ImageAssetType::Pgm;
		case FIF_PGMRAW:
			return ImageAssetType::Pgmraw;
		case FIF_PNG:
			return ImageAssetType::Png;
		case FIF_PPM:
			return ImageAssetType::Ppm;
		case FIF_PPMRAW:
			return ImageAssetType::Ppmraw;
		case FIF_RAS:
			return ImageAssetType::Ras;
		case FIF_TARGA:
			return ImageAssetType::Targa;
		case FIF_TIFF:
			return ImageAssetType::Tiff;
		case FIF_WBMP:
			return ImageAssetType::Wbmp;
		case FIF_PSD:
			return ImageAssetType::Psd;
		case FIF_CUT:
			return ImageAssetType::Cut;
		case FIF_XBM:
			return ImageAssetType::Xbm;
		case FIF_XPM:
			return ImageAssetType::Xpm;
		case FIF_DDS:
			return ImageAssetType::Dds;
		case FIF_GIF:
			return ImageAssetType::Gif;
		case FIF_HDR:
			return ImageAssetType::Hdr;
		case FIF_FAXG3:
			return ImageAssetType::Faxg3;
		case FIF_SGI:
			return ImageAssetType::Sgi;
		case FIF_EXR:
			return ImageAssetType::Exr;
		case FIF_J2K:
			return ImageAssetType::J2k;
		case FIF_JP2:
			return ImageAssetType::Jp2;
		case FIF_PFM:
			return ImageAssetType::Pfm;
		case FIF_PICT:
			return ImageAssetType::Pict;
		case FIF_RAW:
			return ImageAssetType::Raw;
		case FIF_WEBP:
			return ImageAssetType::Webp;
		case FIF_JXR:
			return ImageAssetType::Jxr;
		default:
			return {};
        }
    }
}