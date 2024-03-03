
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

                Entry(String &&name);
                void SetByFileString(const String &set) override final;
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
                String GetFileString() const override final;
                bool IsSection() const override final;
            };
        private:
            static const String fileName;

            typedef std::unique_ptr<EntryBase> EntryPtr;
            std::vector<EntryPtr> entries;

            template<ID id>
            Entry<typename Traits<id>::T>* CreateEntry(String &&name);
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
        Manager::Entry<T>::Entry(String &&name) : name(std::move(name))
        {}

        template<class T>
        void Manager::Entry<T>::SetByFileString(const String &set)
        {
            // First, get past all spaces
            size_t count = 0;
            auto loop = set.begin();
            while (loop != set.end(), *loop == ' ')
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

            value = FromString<T>(retrieveFrom);
        }

        template<class T>
        String Manager::Entry<T>::GetFileString() const
        {
            return name + assignmentToken + ToString(value) + "\n";
        }

        template<class T>
        bool Manager::Entry<T>::IsSection() const
        {
            return false;
        }

        template<ID id>
        Manager::Entry<typename Traits<id>::T>* Manager::CreateEntry(String &&name)
        {
            entries.push_back(EntryPtr(new Entry<Traits<id>::T>(std::move(name))));
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

        /*
        class Handler
        {
        public:
            class Section
            {
            private:
                class LineBase
                {
                public:
                    virtual ~LineBase() = 0 {}
                    virtual String GetString() = 0;
                    virtual void Load(const String &string) = 0;
                };

                template<class T>
                class LineDerived : public Line<T>, public LineBase
                {
                private:
                    String variableName;

                    typedef std::function<T()> GetVarFunc;
                    GetVarFunc getVarFunc;

                    template<class U, typename std::enable_if<std::is_same<U, String>::value, int>::type = 0>
                    String GetString(const U &obj);
                    template<class U, typename std::enable_if<!std::is_same<U, String>::value, int>::type = 0>
                    String GetString(const U &obj);

                    template<class U, typename std::enable_if<std::is_same<U, String>::value, int>::type = 0>
                    void SetVariable(const String &input);
                    template<class U, typename std::enable_if<!std::is_same<U, String>::value, int>::type = 0>
                    void SetVariable(const String &input);
                public:
                    LineDerived(const String &variableName, const GetVarFunc &getVarFunc);
                    String GetString() override;
                    void Load(const String &string) override;

                    const String& GetVariableName() const override;
                };
            private:
                String header;
                typedef std::unique_ptr<LineBase> LinePtr;
                std::vector<LinePtr> lines;

                Section(const String &header);
                void Save(String &string);
                void Load(const String &header, ::inscription::TextInFile &file);
                template<class T, class Func>
                LineDerived<T>* CreateLinePortal(const String &variableName, Line<T> *&receiverLine, const Func &getVarFunc);
            public:
                Section(Section &&arg);
                template<class T>
                LineDerived<T>* CreateLine(const String &variableName, Line<T> *&receiverLine, T(*getVarFunc)());
                template<class T, class Func>
                LineDerived<T>* CreateLine(const String &variableName, Line<T> *&receiverLine, const Func &getVarFunc);
                friend Handler;
            };
        private:
            std::vector<Section> sections;

            static const FileName FILE_NAME;

            Handler() = default;
            Handler(const Handler &arg) = delete;
            Handler& operator=(const Handler &arg) = delete;

            void CreateSections();
            template<class T>
            static T IniFromString(const String &str, const Section::LineDerived<T> &line);
            static agui::Resolution::Size IniFromString(const String &str, const Section::LineDerived<agui::Resolution::Size> &line);
            static Input::Key IniFromString(const String &str, const Section::LineDerived<Input::Key> &line);
        public:
            NullEvent eventPostLoad;

            static Handler& Instance();
            static Section* CreateSection(const String &header);
            static void Save();
            static void Load();
        };

        template<class T>
        template<class U, typename std::enable_if<std::is_same<U, String>::value, int>::type>
        String Handler::Section::LineDerived<T>::GetString(const U &obj)
        {
            return obj;
        }

        template<class T>
        template<class U, typename std::enable_if<!std::is_same<U, String>::value, int>::type>
        String Handler::Section::LineDerived<T>::GetString(const U &obj)
        {
            return ToString(obj);
        }

        template<class T>
        template<class U, typename std::enable_if<std::is_same<U, String>::value, int>::type>
        void Handler::Section::LineDerived<T>::SetVariable(const String &input)
        {
            variable = input;
        }

        template<class T>
        template<class U, typename std::enable_if<!std::is_same<U, String>::value, int>::type>
        void Handler::Section::LineDerived<T>::SetVariable(const String &input)
        {
            variable = Handler::IniFromString(input, *this);
        }

        template<class T>
        Handler::Section::LineDerived<T>::LineDerived(const String &variableName, const GetVarFunc &getVarFunc) : variableName(variableName), getVarFunc(getVarFunc)
        {}

        template<class T>
        String Handler::Section::LineDerived<T>::GetString()
        {
            return variableName + " = " + GetString(getVarFunc());
        }

        template<class T>
        void Handler::Section::LineDerived<T>::Load(const String &string)
        {
            auto delimiterPos = string.find_last_of("=");
            if (string.substr(0, delimiterPos - 1) != variableName)
                return;

            SetVariable<T>(string.substr(delimiterPos + 2));
        }

        template<class T>
        const String& Handler::Section::LineDerived<T>::GetVariableName() const
        {
            return variableName;
        }

        template<class T, class Func>
        Handler::Section::LineDerived<T>* Handler::Section::CreateLinePortal(const String &variableName, Line<T> *&receiverLine, const Func &getVarFunc)
        {
            auto made = new LineDerived<T>(variableName, getVarFunc);
            receiverLine = made;
            lines.push_back(LinePtr(made));
            return made;
        }

        template<class T>
        Handler::Section::LineDerived<T>* Handler::Section::CreateLine(const String &variableName, Line<T> *&receiverLine, T(*getVarFunc)())
        {
            return CreateLinePortal<T>(variableName, receiverLine, getVarFunc);
        }

        template<class T, class Func>
        Handler::Section::LineDerived<T>* Handler::Section::CreateLine(const String &variableName, Line<T> *&receiverLine, const Func &getVarFunc)
        {
            return CreateLinePortal<T>(variableName, receiverLine, getVarFunc);
        }

        template<class T>
        T Handler::IniFromString(const String &str, const Section::LineDerived<T> &line)
        {
            return FromString<T>(str);
        }

        template<>
        struct Component<::agui::System>
        {
            static Line<agui::Resolution::Size> *resolution;

            static void Init();
        };

        typedef Component<::agui::System> GuiSystem;
        */
    }
}