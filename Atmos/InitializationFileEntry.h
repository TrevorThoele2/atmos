#pragma once

#include <functional>

#include "Event.h"
#include "String.h"
#include "StringUtility.h"

namespace Atmos
{
    class InitializationFileEntryBase
    {
    public:
        virtual ~InitializationFileEntryBase() = 0;

        virtual void SetToFileString(const String& set) = 0;
        virtual String FileString() const = 0;
    };

    template<class T>
    class InitializationFileEntry : public InitializationFileEntryBase
    {
    public:
        typedef std::function<T()> FromOutside;
        Event<T> onLoaded;
    public:
        InitializationFileEntry(const String& name, FromOutside&& fromOutside);

        void SetToFileString(const String& set) override;
        String FileString() const override;
    private:
        String name;
        FromOutside fromOutside;
    private:
        static const String assignmentToken;
    };

    template<class T>
    InitializationFileEntry<T>::InitializationFileEntry(const String& name, FromOutside&& fromOutside) :
        name(name), fromOutside(std::move(fromOutside))
    {}

    template<class T>
    void InitializationFileEntry<T>::SetToFileString(const String& set)
    {
        // First, get past all spaces
        size_t count = 0;
        auto loop = set.begin();
        while (loop != set.end() && *loop == ' ')
            ++loop, ++count;

        // If we're already at the end, or the name isn't right here (at the beginning), then leave
        if (loop == set.end() || set.find_first_of(name, count) != count)
            return;

        count += name.size();
        // Now, we need this token
        if (set.find_first_of(assignmentToken, count) != count)
            return;

        count += assignmentToken.size();
        // Now, we assume that the value is here (we can attempt to remove the last character if it's a newline)
        String retrieveFrom(set.substr(count, String::npos));
        auto retrieveFromLoop = --retrieveFrom.end();
        while (*retrieveFromLoop == '\n')
            retrieveFromLoop = --retrieveFrom.erase(retrieveFromLoop);

        auto fromStringValue = FromString<T>(retrieveFrom);
        onLoaded(fromStringValue);
    }

    template<class T>
    String InitializationFileEntry<T>::FileString() const
    {
        auto value = fromOutside();
        return name + assignmentToken + ToString(value) + "\n";
    }

    template<class T>
    const String InitializationFileEntry<T>::assignmentToken = "=";
}