#include <ShlObj.h>
#include <locale>
#include <codecvt>
#include <direct.h>

#include "WindowsFileManager.h"

namespace Atmos::File
{
    void WindowsFileManager::MakeDirectory(const Path& path)
    {
        _mkdir(path.c_str());
    }

    bool WindowsFileManager::RelocateFile(const Path& from, const Path& to)
    {
        RemoveFile(to);
        const auto wasMoved = rename(from.c_str(), to.c_str()) == 0;
        if (wasMoved)
            RemoveFile(from);
        return wasMoved;
    }

    bool WindowsFileManager::RemoveFile(const Path& remove)
    {
        return std::remove(remove.c_str()) == 0;
    }

    Path WindowsFileManager::ExePath() const
    {
        TCHAR full_path[maxPathLength];
        GetModuleFileName(nullptr, full_path, maxPathLength);
        Path path(full_path);
        path.RemoveFileName();
        path.Append(FileSeparator());

        return path;
    }

    Path WindowsFileManager::DataPath() const
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

        PWSTR path;
        SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path);
        Path ret(converter.to_bytes(path));
        ret.Append(FileSeparator() + "AnExWill" + FileSeparator());
        return ret;
    }

    Path WindowsFileManager::TemporaryDirectoryPath() const
    {
        TCHAR buffer[maxPathLength];
        GetTempPath(maxPathLength, buffer);
        Path path(buffer);
        path.RemoveFileName();
            
        return path;
    }

    size_t WindowsFileManager::MaxPathLength() const
    {
        return maxPathLength;
    }

    String WindowsFileManager::FileSeparator() const
    {
        return String("\\");
    }
}