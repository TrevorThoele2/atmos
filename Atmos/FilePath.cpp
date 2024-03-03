
#include "FilePath.h"

#include "RelativeFilePath.h"

#include "Environment.h"
#include "StringUtility.h"
#include <Inscription\Scribe.h>

namespace Atmos
{
    const char *worldFolder = "Worlds";

    FilePath::FilePath(const String &string) : value(string), extension(GetFileExtension(string))
    {}

    FilePath::FilePath(const char *buf) : value(buf), extension(GetFileExtension(buf))
    {}

    FilePath::FilePath(const FilePath &arg, const RelativeFilePath &relative) : value(arg.value)
    {
        Append(relative);
    }

    FilePath::FilePath(const FilePath &arg) : value(arg.value), extension(arg.extension)
    {}

    FilePath::FilePath(FilePath &&arg) : value(std::move(arg.value)), extension(std::move(arg.extension))
    {}

    FilePath& FilePath::operator=(const FilePath &arg)
    {
        value = arg.value;
        extension = arg.extension;
        return *this;
    }

    FilePath& FilePath::operator=(FilePath &&arg)
    {
        value = std::move(arg.value);
        extension = std::move(arg.extension);
        return *this;
    }

    bool FilePath::operator==(const FilePath &arg) const
    {
        return value == arg.value;
    }

    bool FilePath::operator!=(const FilePath &arg) const
    {
        return !(*this == arg);
    }

    FilePath::operator const char*()
    {
        return value.c_str();
    }

    const char* FilePath::c_str() const
    {
        return value.c_str();
    }

    FilePath& FilePath::Set(const String &set)
    {
        value = set;
        extension = GetFileExtension(value);

        return *this;
    }

    FilePath& FilePath::SetFileName(const FileName &name)
    {
        auto found = value.find_last_of(Environment::GetFileSystem()->GetFileSeparator());
        if (found == value.npos)
        {
            value = name;
            extension = GetFileExtension(value);
            return *this;
        }

        value.replace(found + 1, value.npos, name);
        extension = GetFileExtension(value);

        return *this;
    }

    FilePath& FilePath::SetExtension(const String &replace)
    {
        value = ReplaceFileExtension(value, replace);
        extension = replace;

        return *this;
    }

    FilePath& FilePath::RemoveFileName()
    {
        value = ::Atmos::RemoveFileName(value);
        extension = "";

        return *this;
    }

    FilePath& FilePath::RemoveExtension()
    {
        value = RemoveFileExtension(value);
        extension = "";

        return *this;
    }

    FilePath& FilePath::RemoveDirectory(size_t index)
    {
        if (index >= GetDirectoryCount())
            return *this;

        String directoryName = GetDirectoryName(index);
        value.erase(value.find(directoryName), directoryName.size());

        return *this;
    }

    FilePath& FilePath::Append(const String &append)
    {
        value.append(append);
        extension = GetFileExtension(value);
        return *this;
    }

    FilePath& FilePath::Append(const FileName& append)
    {
        return Append(append.GetValue());
    }

    FilePath& FilePath::Append(const RelativeFilePath &append)
    {
        RemoveFileName();
        size_t directoryIndex = GetDirectoryCount() - 1;
        for (size_t loop = 0; loop < append.GetMoveUpCount(); ++loop, --directoryIndex)
            RemoveDirectory(directoryIndex);

        Append(append.GetMoveDown());
        return *this;
    }

    FilePath& FilePath::Append(const char* append)
    {
        return Append(String(append));
    }

    FilePath& FilePath::AppendSeparator()
    {
        Append(Environment::GetFileSystem()->GetFileSeparator());
        return *this;
    }

    FilePath& FilePath::ReplaceAllWith(const String &replace, const String &with)
    {
        for (size_t index = 0; index < value.length() - replace.length(); ++index)
            if (value.substr(index, replace.length()) == replace)
                value.replace(index, replace.length(), with);

        return *this;
    }

    bool FilePath::HasDirectory(const String &directoryName)
    {
        size_t curPos = 0;
        // Retrieve folders from front to back until folderName is found or not
        String slice;
        while (true)
        {
            auto found = value.find(Environment::GetFileSystem()->GetFileSeparator(), curPos);
            if (found == value.npos)
                return false;

            slice = value.substr(curPos, found);
            if (slice == directoryName)
                return true;

            curPos = found;
        }

        return false;
    }

    size_t FilePath::GetDirectoryCount() const
    {
        size_t count = 0;
        if (value.find_first_of(Environment::GetFileSystem()->GetFileSeparator()) != 0)
            ++count;

        size_t position = value.find(Environment::GetFileSystem()->GetFileSeparator());
        while (position != value.npos)
        {
            ++count;
            position = value.find(Environment::GetFileSystem()->GetFileSeparator(), position + 1);
        }

        return count;
    }

    String FilePath::GetDirectoryName(size_t index) const
    {
        if (index >= GetDirectoryCount())
            return String();

        size_t positionStart = 0;
        size_t positionEnd = value.find(Environment::GetFileSystem()->GetFileSeparator(), positionStart + 1);
        for(size_t loop = 0; loop < index; ++loop)
        {
            positionStart = positionEnd;
            positionEnd = value.find(Environment::GetFileSystem()->GetFileSeparator(), positionEnd + 1);
        }

        return value.substr(positionStart, positionEnd - positionStart);
    }

    FilePath::operator String() const
    {
        return value;
    }

    String FilePath::GetValue() const
    {
        return value;
    }

    String FilePath::GetExtension() const
    {
        return extension;
    }

    String FilePath::GetWithoutExtension() const
    {
        return RemoveFileExtension(value);
    }

    bool FilePath::IsEmpty() const
    {
        return value == "";
    }

    FileName FilePath::GetFileName() const
    {
        auto found = value.find_last_of(Environment::GetFileSystem()->GetFileSeparator());
        if (found == value.npos)
            return FileName(value);

        return FileName(value.substr(found + 1));
    }

    void FilePath::Serialize(::Inscription::Scribe &scribe)
    {
        scribe(value);
        if (scribe.IsInput())
            extension = GetFileExtension(value);
    }

    FileName::FileName(const String &string) : value(string), extension(GetFileExtension(string))
    {}

    FileName::FileName(const char *buf) : value(buf), extension(GetFileExtension(buf))
    {}

    FileName::FileName(const FilePath &path) : value(path.GetFileName()), extension(path.GetExtension())
    {}

    FileName::FileName(const FileName &arg) : value(arg.value), extension(arg.extension)
    {}

    FileName::FileName(FileName &&arg) : value(std::move(arg.value)), extension(std::move(arg.extension))
    {}

    FileName& FileName::operator=(const FileName &arg)
    {
        value = arg.value;
        extension = arg.extension;
        return *this;
    }

    FileName& FileName::operator=(FileName &&arg)
    {
        value = std::move(arg.value);
        extension = std::move(arg.extension);
        return *this;
    }

    bool FileName::operator==(const FileName &arg) const
    {
        return value == arg.value;
    }

    bool FileName::operator!=(const FileName &arg) const
    {
        return !(*this == arg);
    }

    FileName::operator const String&()
    {
        return value;
    }

    FileName::operator const char*()
    {
        return value.c_str();
    }

    const char* FileName::c_str() const
    {
        return value.c_str();
    }

    FileName& FileName::Set(const String &set)
    {
        value = set;
        extension = GetFileExtension(value);

        return *this;
    }

    FileName& FileName::SetExtension(const String &replace)
    {
        value = ReplaceFileExtension(value, replace);
        extension = replace;

        return *this;
    }

    FileName::operator const String&() const
    {
        return value;
    }

    const String& FileName::GetValue() const
    {
        return value;
    }

    const String& FileName::GetExtension() const
    {
        return extension;
    }

    String FileName::GetWithoutExtension() const
    {
        return RemoveFileExtension(value);
    }

    bool FileName::IsEmpty() const
    {
        return value == "";
    }

    void FileName::Serialize(::Inscription::Scribe &scribe)
    {
        scribe(value);
        if (scribe.IsInput())
            extension = GetFileExtension(value);
    }
}