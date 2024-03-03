#include <ShlObj.h>
#include <locale>
#include <codecvt>
#include <direct.h>

#include "WindowsFileManager.h"

namespace Atmos::File
{
    Path WindowsFileManager::DataPath() const
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

        PWSTR path;
        SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path);
        Path ret(converter.to_bytes(path));
        ret.append("AnExWill");
        return ret;
    }

    Path WindowsFileManager::TemporaryDirectoryPath() const
    {
        TCHAR buffer[maxPathLength];
        GetTempPath(maxPathLength, buffer);
        Path path(buffer);
        path.remove_filename();
            
        return path;
    }

    size_t WindowsFileManager::MaxPathLength() const
    {
        return maxPathLength;
    }
}