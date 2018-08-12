
#include "RelativeFilePath.h"

#include "FilePath.h"

#include "Environment.h"
#include "StringUtility.h"

namespace Atmos
{
    RelativeFilePath::RelativeFilePath() : moveUp(0)
    {}

    RelativeFilePath::RelativeFilePath(const String &moveDown) : moveDown(moveDown), extension(GetFileExtension(moveDown)), moveUp(0)
    {}

    RelativeFilePath::RelativeFilePath(const char *moveDown) : moveDown(moveDown), extension(GetFileExtension(moveDown)), moveUp(0)
    {}

    RelativeFilePath::RelativeFilePath(const FilePath &from, const FilePath &to) : moveUp(0)
    {
        bool foundDifference = false;
        String fromValue = from.GetValue();
        String toValue = to.GetValue();
        for (size_t loop = 0; loop < from.GetDirectoryCount(); ++loop)
        {
            String fromDirectoryName = from.GetDirectoryName(loop);
            if (fromDirectoryName == to.GetDirectoryName(loop))
                continue;

            toValue = toValue.substr(toValue.find(fromDirectoryName));
            foundDifference = true;
            break;
        }

        if (foundDifference)
        {
            FilePath newFilePath(toValue);
            moveUp = newFilePath.GetDirectoryCount() - 1;
            moveDown = newFilePath.GetValue();
        }
        else
            moveDown = to.GetValue().substr(fromValue.size());

        extension = GetFileExtension(moveDown);
    }

    RelativeFilePath::RelativeFilePath(const RelativeFilePath &arg) : moveDown(arg.moveDown), moveUp(arg.moveUp), extension(arg.extension)
    {}

    RelativeFilePath::RelativeFilePath(RelativeFilePath &&arg) : moveDown(std::move(arg.moveDown)), moveUp(std::move(arg.moveUp)), extension(std::move(arg.extension))
    {}

    RelativeFilePath& RelativeFilePath::operator=(const RelativeFilePath &arg)
    {
        moveDown = arg.moveDown;
        moveUp = arg.moveUp;
        extension = arg.extension;
        return *this;
    }

    RelativeFilePath& RelativeFilePath::operator=(RelativeFilePath &&arg)
    {
        moveDown = std::move(arg.moveDown);
        moveUp = std::move(arg.moveUp);
        extension = std::move(arg.extension);
        return *this;
    }

    bool RelativeFilePath::operator==(const RelativeFilePath &arg) const
    {
        return moveDown == arg.moveDown && moveUp == arg.moveUp;
    }

    bool RelativeFilePath::operator!=(const RelativeFilePath &arg) const
    {
        return !(*this == arg);
    }

    RelativeFilePath::operator const char*()
    {
        return GetValue().c_str();
    }

    const char* RelativeFilePath::c_str() const
    {
        return GetValue().c_str();
    }

    void RelativeFilePath::SetName(const FileName &name)
    {
        auto found = moveDown.find_last_of(Environment::GetFileSystem()->GetFileSeparator());
        if (found == moveDown.npos)
        {
            moveDown = name;
            extension = GetFileExtension(moveDown);
            return;
        }

        moveDown.replace(found + 1, moveDown.npos, name);
        extension = GetFileExtension(moveDown);
    }

    void RelativeFilePath::SetExtension(const String &replace)
    {
        moveDown = ReplaceFileExtension(moveDown, replace);
        extension = replace;
    }

    void RelativeFilePath::RemoveFileName()
    {
        moveDown = ::Atmos::RemoveFileName(moveDown);
        extension = "";
    }

    void RelativeFilePath::RemoveExtension()
    {
        moveDown = RemoveFileExtension(moveDown);
        extension = "";
    }

    RelativeFilePath& RelativeFilePath::Up(size_t number)
    {
        moveUp += number;
        return *this;
    }

    RelativeFilePath& RelativeFilePath::Down(const String &down)
    {
        moveDown += down;
        return *this;
    }

    size_t RelativeFilePath::GetMoveUpCount() const
    {
        return moveUp;
    }

    String RelativeFilePath::GetMoveDown() const
    {
        return moveDown;
    }

    bool RelativeFilePath::HasDirectory(const String &directoryName)
    {
        size_t curPos = 0;
        // Retrieve folders from front to back until folderName is found or not
        String slice;
        while (true)
        {
            auto found = moveDown.find(Environment::GetFileSystem()->GetFileSeparator(), curPos);
            if (found == moveDown.npos)
                return false;

            slice = moveDown.substr(curPos, found);
            if (slice == directoryName)
                return true;

            curPos = found;
        }

        return false;
    }

    size_t RelativeFilePath::GetDirectoryCount() const
    {
        size_t count = 0;
        if (moveDown.find_first_of(Environment::GetFileSystem()->GetFileSeparator()) != 0)
            ++count;

        size_t position = moveDown.find(Environment::GetFileSystem()->GetFileSeparator());
        while (position != moveDown.npos)
        {
            ++count;
            position = moveDown.find(Environment::GetFileSystem()->GetFileSeparator(), position + 1);
        }

        return count;
    }

    String RelativeFilePath::GetDirectoryName(size_t index) const
    {
        if (index >= GetDirectoryCount())
            return String();

        size_t positionStart = 0;
        size_t positionEnd = moveDown.find(Environment::GetFileSystem()->GetFileSeparator(), positionStart + 1);
        for (size_t loop = 0; loop < index; ++loop)
        {
            positionStart = positionEnd;
            positionEnd = moveDown.find(Environment::GetFileSystem()->GetFileSeparator(), positionEnd + 1);
        }

        return moveDown.substr(positionStart, positionEnd - positionStart);
    }

    RelativeFilePath::operator String() const
    {
        return GetValue();
    }

    String RelativeFilePath::GetValue() const
    {
        String ret;
        for (size_t loop = 0; loop < moveUp; ++loop)
            ret += "..\\";
        ret += moveDown;
        return ret;
    }

    String RelativeFilePath::GetExtension() const
    {
        return extension;
    }

    String RelativeFilePath::GetWithoutExtension() const
    {
        return RemoveFileExtension(moveDown);
    }

    bool RelativeFilePath::IsEmpty() const
    {
        return moveDown == "" && moveUp == 0;
    }

    FileName RelativeFilePath::GetFileName() const
    {
        auto found = moveDown.find_last_of(Environment::GetFileSystem()->GetFileSeparator());
        if (found == moveDown.npos)
            return FileName(moveDown);

        return FileName(moveDown.substr(found + 1));
    }

    void RelativeFilePath::Serialize(::Inscription::Scribe &scribe)
    {
        scribe(moveUp);
        scribe(moveDown);
        if (scribe.IsInput())
            extension = GetFileExtension(moveDown);
    }
}