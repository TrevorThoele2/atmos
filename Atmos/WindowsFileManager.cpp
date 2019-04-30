
#include <ShlObj.h>
#include <locale>
#include <codecvt>
#include <direct.h>

#include "WindowsFileManager.h"

namespace Atmos
{
    void WindowsFileManager::MakeDirectory(const FilePath& path)
    {
        _mkdir(path.c_str());
    }

    bool WindowsFileManager::RelocateFile(const FilePath& from, const FilePath& to)
    {
        RemoveFile(to);
        bool wasMoved = rename(from.c_str(), to.c_str()) == 0;
        if (wasMoved)
            RemoveFile(from);
        return wasMoved;
    }

    bool WindowsFileManager::RemoveFile(const FilePath& remove)
    {
        return std::remove(remove.c_str()) == 0;
    }

    FilePath WindowsFileManager::ExePath() const
    {
        TCHAR full_path[maxPathLength];
        GetModuleFileName(nullptr, full_path, maxPathLength);
        FilePath path(full_path);
        path.RemoveFileName();
        path.Append(FileSeparator());

        return path;
    }

    FilePath WindowsFileManager::DataPath() const
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

        PWSTR path;
        SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path);
        FilePath ret(converter.to_bytes(path));
        ret.Append(FileSeparator() + "AnExWill" + FileSeparator());
        return ret;
    }

    FilePath WindowsFileManager::TemporaryDirectoryPath() const
    {
        TCHAR buffer[maxPathLength];
        GetTempPath(maxPathLength, buffer);
        FilePath path(buffer);
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