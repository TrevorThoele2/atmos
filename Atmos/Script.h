
#pragma once

#include <memory>
#include <vector>

#include "Asset.h"
#include "ScriptItem.h"
#include "ScriptCaller.h"

#include "FilePath.h"
#include "Variant.h"

#include "Serialization.h"
#include <Inscription\TrackingTraits.h>

namespace Falcon
{
    class VMachine;
    class VMachineWrapper;
    class Item;
    class Module;
    class Runtime;
}

namespace Atmos
{
    class ScriptException : public std::runtime_error
    {
    public:
        ScriptException() : runtime_error("A generic script exception has occured.") {}
    };

    class ScriptModule;
    class ScriptModuleBase : public Asset
    {
    public:
        typedef std::vector<ScriptModule*> Dependencies;
    public:
        ScriptModuleBase() = default;
        ScriptModuleBase(const Dependencies &dependencies);
        ScriptModuleBase(Dependencies &&dependencies);
        ScriptModuleBase(const ScriptModuleBase &arg) = default;
        ScriptModuleBase& operator=(const ScriptModuleBase &arg) = default;
        virtual ~ScriptModuleBase() = 0 {}

        bool operator==(const ScriptModuleBase &arg) const;
        bool operator!=(const ScriptModuleBase &arg) const;

        void AddDependency(ScriptModule &add);
        // Retrieves the file name from the ScriptRegistry. If it does not exist, will just return
        void AddDependency(const FileName &fileName);
        void RemoveDependency(ScriptModule &remove);
        void RemoveDependency(const FileName &fileName);
        const Dependencies& GetDependencies() const;

        virtual const FileName& GetFileName() const = 0;
        virtual Falcon::Module& GetBaseModule() = 0;

        virtual bool IsModule() const = 0;
        virtual bool IsScript() const = 0;

        static FileName& SetupModuleExtension(FileName &fileName);
        static FilePath& SetupModuleExtension(FilePath &filePath);
    protected:
        Dependencies dependencies;
    private:
        String GetStringImpl() const override final;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };

    template<class T>
    class AssetRegistry;

    // Do not create this directly. Create from inside of ScriptRegistry
    class ScriptModule : public ScriptModuleBase
    {
    public:
        ScriptModule(Falcon::Module &wrapped, const FileName &fileName);
        ScriptModule(Falcon::Module &wrapped, const FileName &fileName, const Dependencies &dependencies);
        ScriptModule(Falcon::Module &wrapped, const FileName &fileName, Dependencies &&dependencies);
        ScriptModule(ScriptModule &&arg);

        bool operator==(const ScriptModule &arg) const;
        bool operator!=(const ScriptModule &arg) const;

        operator Falcon::Module&();
        operator Falcon::Module&() const;

        const FileName& GetFileName() const override;
        Falcon::Module& GetBaseModule() override;

        bool IsModule() const override final;
        bool IsScript() const override final;
    private:
        Falcon::Module &wrapped;
        FileName fileName;
        friend class AssetRegistry<ScriptModuleBase>;
    };

    class Script : public ScriptModuleBase
    {
    public:
        typedef Name SymbolName;
    private:
        class Impl;
    public:
        class Instance
        {
        public:
            typedef Script WrappedT;
            typedef WrappedT::Dependencies Dependencies;
            typedef WrappedT::SymbolName SymbolName;
            typedef std::vector<ScriptItem> ItemVector;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        private:
            friend class ScriptController;
        private:
            class Impl;
        private:
            std::shared_ptr<Instance::Impl> impl;
            bool suspended;

            ItemVector globalItems;

            Script::SymbolName executeName;
            ItemVector parameters;

            ScriptCaller caller;

            void InitializeImpl();
            void SetupForCleanExecution();
            void SetupForResumingExecution();
        public:
            Instance();
            Instance(const Script &script);
            Instance(const Instance &arg);
            Instance(Instance &&arg);
            Instance& operator=(const Instance &arg);
            Instance& operator=(Instance &&arg);
            ~Instance();

            bool operator==(const Instance &arg) const;
            bool operator!=(const Instance &arg) const;

            void Set();
            void Set(const Script &script);

            bool IsValid() const;
            explicit operator bool() const;

            const FileName& GetFileName() const;

            // Executes the script standardly
            // Will defer the execute until a certain point in the frame (probably a bit later)
            void ExecuteDeferred();
            // Executes a sub-routine inside of the script
            // Will defer the execute until a certain point in the frame (probably a bit later)
            void ExecuteDeferred(const SymbolName &overrideExecuteName, const Script::Instance::ItemVector &overrideParameters);
            // Executes the script standardly
            // Will immediately execute the script
            void ExecuteImmediately();
            // Executes a sub-routine inside of the script
            // Will immediately execute the script
            void ExecuteImmediately(const SymbolName &overrideExecuteName, const Script::Instance::ItemVector &overrideParameters);
            // Resumes the script from being suspended
            void Resume();
            // Suspends the script for the rest of the frame
            void Suspend();
            void ForceQuit();
            // Returns if this script is currently running
            // NOTE: Suspension has no effect on this
            bool IsExecuting() const;
            bool IsSuspended() const;

            Falcon::VMachineWrapper& GetVM();

            void AddGlobalItem(const ScriptItem &add);
            void RemoveGlobalItem(size_t index);
            void RemoveGlobalItem(const Name &name);
            ScriptItem* FindGlobalItem(const Name &name);
            const ScriptItem* FindGlobalItem(const Name &name) const;
            ScriptItem* FindGlobalItem(size_t index);
            const ScriptItem* FindGlobalItem(size_t index) const;
            const ItemVector& GetGlobalItems() const;
            // If the global item exists, then sets it. Otherwise adds it and sets it to the variant
            ScriptItem* SetOrAddGlobalItem(const Name &name, const Variant &variant);

            // Resets the execute name
            void SetExecuteName();
            // Sets the execute name
            void SetExecuteName(const Script::SymbolName &set);
            // Gets the execute name
            const Script::SymbolName& GetExecuteName() const;

            // Only valid whenever the execute name is set
            void AddParameter(const ScriptItem &add);
            void RemoveParameter(size_t index);
            void RemoveParameter(const Name &name);
            ScriptItem* FindParameter(const Name &name);
            const ScriptItem* FindParameter(const Name &name) const;
            ScriptItem* FindParameter(size_t index);
            const ScriptItem* FindParameter(size_t index) const;
            const ItemVector& GetParameters() const;
            ScriptCaller& GetCaller();
            const ScriptCaller& GetCaller() const;
        };
    private:
        std::unique_ptr<Impl> impl;
    public:
        Script(Dependencies &&dependencies, const FilePath &filePath);
        Script(Falcon::Module &wrapped, const FileName &fileName);
        Script(const Script &arg);
        Script(Script &&arg);
        Script& operator=(const Script &arg);
        Script& operator=(Script &&arg);

        bool operator==(const Script &arg) const;
        bool operator!=(const Script &arg) const;

        Instance MakeInstance() const;

        const FileName& GetFileName() const override;
        Falcon::Module& GetBaseModule() override;

        bool IsModule() const override final;
        bool IsScript() const override final;
    private:
        INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(Script, private);
    };
}

namespace Inscription
{
    template<>
    class Inscripter<::Atmos::Script> : public InscripterBase<::Atmos::Script>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_INSCRIPTER_DECLARE_SERIALIZE_FUNCTION;
    };
}

namespace Atmos
{
    class ScriptCompiler
    {
    public:
        // Returns true if the script was successfully set
        // Can fail if the script-to-set has an error
        static bool SetScriptToCompile(const FilePath &filePath);
        // Returns true if the script was successfully set
        // Can fail if the script-to-set has an error
        static bool SetScriptToCompile(const FilePath &filePath, const ScriptModule::Dependencies &dep);
        // Returns true if the script was successfully set
        // Can fail if the script-to-set has an error
        static bool SetScriptToCompile(Script &script);
        // Returns true if the script was successfully set
        // Can fail if the script-to-set has an error
        static bool SetScriptToCompile(Script &script, const ScriptModule::Dependencies &dep);
        // The script to compile must be setup already
        static void AddDependency(const FileName &fileName);
        static void AddDependency(ScriptModule &module);
        // Returns true if the script was successfully compiled
        static bool Compile();
        static void Reset();
    private:
        static std::unique_ptr<ScriptModule> inFocus;

        ScriptCompiler() = delete;
        ScriptCompiler(const ScriptCompiler &arg) = delete;
        ScriptCompiler& operator=(const ScriptCompiler &arg) = delete;

        static void OutputNoFilePathCompilationError();
        static void OutputGeneralCompilationError(const String &err);
    };
}