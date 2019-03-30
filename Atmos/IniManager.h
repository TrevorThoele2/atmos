
#pragma once

#include <vector>
#include <memory>
#include "String.h"
#include "Event.h"

#include "IniTraits.h"

namespace Atmos
{
    namespace Ini
    {
        class Manager
        {
        public:
            class EntryBase
            {
            public:
                virtual ~EntryBase();
                virtual void SetByFileString(const String &set) = 0;
                virtual void SetFromInternal() = 0;
                virtual String GetFileString() const = 0;
                virtual bool IsSection() const = 0;
            };

            template<class T>
            class Entry : public EntryBase
            {
            private:
                static const String assignmentToken;
            public:
                String name;
                T value;
                std::function<T()> valueGetter;

                Entry(String &&name, std::function<T()> &&valueGetter);
                void SetByFileString(const String &set) override final;
                void SetFromInternal() override final;
                String GetFileString() const override final;
                bool IsSection() const override final;
            };
        private:
            class EntrySection : public EntryBase
            {
            public:
                bool first;
                String string;

                EntrySection(String &&string, bool first);
                void SetByFileString(const String &set) override final;
                void SetFromInternal() override final;
                String GetFileString() const override final;
                bool IsSection() const override final;
            };
        private:
            static const String fileName;

            typedef std::unique_ptr<EntryBase> EntryPtr;
            std::vector<EntryPtr> entries;

            template<ID id>
            Entry<typename Traits<id>::T>* CreateEntry(String &&name, std::function<typename Traits<id>::T()> &&valueGetter);
        public:
            NullEvent eventPostLoad;

            void Init();

            void Save();
            void Load();

            template<ID id>
            typename Traits<id>::T* GetEntry();
            template<ID id>
            typename const Traits<id>::T* GetEntry() const;
            EntryBase* GetEntry(ID id);
            const EntryBase* GetEntry(ID id) const;
            typedef Entry<Atmos::Input::KeyID> InputEntry;
            std::vector<InputEntry*> GetInputTotal();
        };

        template<class T>
        const String Manager::Entry<T>::assignmentToken = " = ";

        template<class T>
        Manager::Entry<T>::Entry(String &&name, std::function<T()> &&valueGetter) : name(std::move(name)), valueGetter(std::move(valueGetter))
        {}

        template<class T>
        void Manager::Entry<T>::SetByFileString(const String &set)
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

            auto &fromStringValue = FromIniString<T>(retrieveFrom);
            if(fromStringValue)
                value = fromStringValue.Get();
        }

        template<class T>
        void Manager::Entry<T>::SetFromInternal()
        {
            value = valueGetter();
        }

        template<class T>
        String Manager::Entry<T>::GetFileString() const
        {
            return name + assignmentToken + ToIniString(value) + "\n";
        }

        template<class T>
        bool Manager::Entry<T>::IsSection() const
        {
            return false;
        }

        template<ID id>
        Manager::Entry<typename Traits<id>::T>* Manager::CreateEntry(String &&name, std::function<typename Traits<id>::T()> &&valueGetter)
        {
            entries.push_back(EntryPtr(new Entry<Traits<id>::T>(std::move(name), std::move(valueGetter))));
            return static_cast<Entry<Traits<id>::T>*>(entries.back().get());
        }

        template<ID id>
        typename Traits<id>::T* Manager::GetEntry()
        {
            return &static_cast<Entry<Traits<id>::T>*>(entries[static_cast<size_t>(id)].get())->value;
        }

        template<ID id>
        typename const Traits<id>::T* Manager::GetEntry() const
        {
            return &static_cast<const Entry<Traits<id>::T>*>(entries[static_cast<size_t>(id)].get())->value;
        }
    }
}