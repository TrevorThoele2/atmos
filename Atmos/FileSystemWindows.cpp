
#include <ShlObj.h>
#include <locale>
#include <codecvt>
#include "FileSystemWindows.h"

namespace Atmos
{
    namespace FileSystem
    {
        bool WindowsHandler::RelocateFile(const FilePath &from, const FilePath &to)
        {
            RemoveFile(to);
            bool wasMoved = rename(from.c_str(), to.c_str()) == 0;
            if (wasMoved)
                RemoveFile(from);
            return wasMoved;
        }

        bool WindowsHandler::RemoveFile(const FilePath &remove)
        {
            return std::remove(remove.c_str()) == 0;
        }

        FilePath WindowsHandler::GetExePath() const
        {
            TCHAR full_path[maxPathLength];
            GetModuleFileName(nullptr, full_path, maxPathLength);
            FilePath path(full_path);
            path.RemoveFileName();
            path.Append(GetFileSeparator());

            return path;
        }

        FilePath WindowsHandler::GetDataPath() const
        {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

            PWSTR path;
            SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path);
            FilePath ret(converter.to_bytes(path));
            ret.Append(GetFileSeparator() + "Without" + GetFileSeparator());
            return ret;
        }

        FilePath WindowsHandler::GetTempDirectoryPath() const
        {
            TCHAR buffer[maxPathLength];
            GetTempPath(maxPathLength, buffer);
            FilePath path(buffer);
            path.RemoveFileName();
            
            return path;
        }

        size_t WindowsHandler::GetMaxPathLength() const
        {
            return maxPathLength;
        }

        String WindowsHandler::GetFileSeparator() const
        {
            return String("\\");
        }
    }
}