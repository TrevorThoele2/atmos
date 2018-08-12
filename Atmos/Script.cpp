
#include "Script.h"

#include "ScriptRegistry.h"
#include "ScriptController.h"
#include "ScriptLoader.h"
#include "FalconScriptUtility.h"

#include "Logger.h"
#include "Assert.h"

#include <falcon\engine.h>
#include <falcon\module.h>

#include <Inscription\Inscripter.h>
#include <Inscription\Scribe.h>
#include <Inscription\Vector.h>
#include "SimpleFile.h"

#include <Chroma/Iterate.h>

namespace Atmos
{
    ScriptModuleBase::ScriptModuleBase(const Dependencies &dependencies) : dependencies(dependencies)
    {}

    ScriptModuleBase::ScriptModuleBase(Dependencies &&dependencies) : dependencies(std::move(dependencies))
    {}

    bool ScriptModuleBase::operator==(const ScriptModuleBase &arg) const
    {
        return Asset::operator==(arg);
    }

    bool ScriptModuleBase::operator!=(const ScriptModuleBase &arg) const
    {
        return !(*this == arg);
    }

    void ScriptModuleBase::AddDependency(ScriptModule &add)
    {
        dependencies.push_back(&add);
    }

    void ScriptModuleBase::AddDependency(const FileName &fileName)
    {
        auto found = AssetRegistry<ScriptModuleBase>::Find(fileName);
        if (found && found->IsModule())
            dependencies.push_back(const_cast<ScriptModule*>(static_cast<const ScriptModule*>(found.Get())));
    }

    void ScriptModuleBase::RemoveDependency(ScriptModule &remove)
    {
        for (auto loop = dependencies.begin(); loop != dependencies.end(); ++loop)
        {
            if (&remove == *loop)
            {
                dependencies.erase(loop);
                return;
            }
        }
    }

    void ScriptModuleBase::RemoveDependency(const FileName &fileName)
    {
        for (auto loop = dependencies.begin(); loop != dependencies.end(); ++loop)
        {
            if ((*loop)->GetFileName() == fileName)
            {
                dependencies.erase(loop);
                return;
            }
        }
    }

    const ScriptModuleBase::Dependencies& ScriptModuleBase::GetDependencies() const
    {
        return dependencies;
    }

    FileName& ScriptModuleBase::SetupModuleExtension(FileName &fileName)
    {
        fileName.SetExtension("fam");
        return fileName;
    }

    FilePath& ScriptModuleBase::SetupModuleExtension(FilePath &filePath)
    {
        filePath.SetExtension("fam");
        return filePath;
    }

    String ScriptModuleBase::GetStringImpl() const
    {
        return GetFileName().GetValue();
    }

    void ScriptModuleBase::Serialize(::Inscription::Scribe &scribe)
    {
        if (scribe.IsOutput())
        {
            ::Inscription::ContainerSize size(dependencies.size());
            scribe.Save(size);

            for (auto &loop : dependencies)
                scribe.AssumeExists(loop);
        }
        else
        {
            ::Inscription::ContainerSize size;
            scribe.Load(size);

            while (size-- > 0)
            {
                ScriptModule *module;
                scribe.AssumeExists(module);
            }
        }
    }

    ScriptModule::ScriptModule(Falcon::Module &wrapped, const FileName &fileName) : wrapped(wrapped), fileName(fileName)
    {
        ScriptModuleBase::SetupModuleExtension(this->fileName);
    }

    ScriptModule::ScriptModule(Falcon::Module &wrapped, const FileName &fileName, const Dependencies &dependencies) : ScriptModuleBase(dependencies), wrapped(wrapped), fileName(fileName)
    {
        ScriptModuleBase::SetupModuleExtension(this->fileName);
    }

    ScriptModule::ScriptModule(Falcon::Module &wrapped, const FileName &fileName, Dependencies &&dependencies) : ScriptModuleBase(std::move(dependencies)), wrapped(wrapped), fileName(fileName)
    {
        ScriptModuleBase::SetupModuleExtension(this->fileName);
    }

    ScriptModule::ScriptModule(ScriptModule &&arg) : ScriptModuleBase(std::move(arg)), wrapped(arg.wrapped), fileName(std::move(arg.fileName))
    {}

    bool ScriptModule::operator==(const ScriptModule &arg) const
    {
        return ScriptModuleBase::operator==(arg);
    }

    bool ScriptModule::operator!=(const ScriptModule &arg) const
    {
        return !(*this == arg);
    }

    ScriptModule::operator Falcon::Module&()
    {
        return wrapped;
    }

    ScriptModule::operator Falcon::Module&() const
    {
        return wrapped;
    }

    const FileName& ScriptModule::GetFileName() const
    {
        return fileName;
    }

    Falcon::Module& ScriptModule::GetBaseModule()
    {
        return wrapped;
    }

    bool ScriptModule::IsModule() const
    {
        return true;
    }

    bool ScriptModule::IsScript() const
    {
        return false;
    }

    void SetupRuntimeDependencies(Falcon::Runtime &runtime, const ScriptModuleBase::Dependencies &dependencies)
    {
        for (auto &loop : dependencies)
        {
            SetupRuntimeDependencies(runtime, loop->GetDependencies());
            runtime.addModule(&loop->GetBaseModule());
        }
    }

    class Script::Impl
    {
    public:
        FileName fileName;
        Falcon::Module *module;

        Impl(Falcon::Module &wrapped, const FileName &fileName) : module(&wrapped), fileName(fileName)
        {}

        Impl(const FilePath &filePath) : fileName(filePath), module(ScriptLoader::GetModuleLoader().loadFile(filePath.c_str()))
        {}
    };

    class Script::Instance::Impl
    {
    public:
        FileName fileName;

        Falcon::VMachineWrapper vMachine;
        Falcon::Module *module;
        const Dependencies *dependencies;
        const ItemVector *globalItems;

        bool isInitialized;

        Impl(Falcon::Module &wrapped, const FileName &fileName) : module(&wrapped), fileName(fileName), isInitialized(false)
        {}

        Impl(const Impl &arg) : fileName(arg.fileName), module(arg.module), isInitialized(arg.isInitialized)
        {}

        Impl& operator=(const Impl &arg)
        {
            fileName = arg.fileName;
            module = arg.module;
            isInitialized = arg.isInitialized;
            return *this;
        }

        void Init()
        {
            if (isInitialized)
                return;

            try
            {
                Falcon::Runtime runtime(&ScriptLoader::GetModuleLoader());

                SetupRuntimeDependencies(runtime, *dependencies);

                runtime.addModule(module);

                // Do the linking
                vMachine->link(Falcon::core_module_init());
                vMachine->link(ScriptLoader::GetMainModule());

                vMachine->link(&runtime);

                // Set the global items
                //for (auto &loop : *globalItems)
                   //Fal::SetItem(*vMachine.vm(), *vMachine->findGlobalItem(Fal::Convert(loop.GetName())), loop.GetValue());
            }
            catch (Falcon::Error *err)
            {
                Falcon::AutoCString edesc(err->toString());
                Logger::Log(String("Script initialization has encountered an error.\n") + edesc.c_str(),
                    Logger::Type::ERROR_MODERATE,
                    Logger::NameValueVector{ NameValuePair("File Name", fileName.GetValue()) });
                isInitialized = false;
                return;
            }

            isInitialized = true;
        }
    };

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Script::Instance)
    {
        if (scribe.IsOutput())
        {
            bool implExists = impl.get();
            scribe.Save(implExists);
            if(implExists)
                scribe.Save(impl->fileName);
        }
        else // INPUT
        {
            bool implExists;
            scribe.Load(implExists);
            if (implExists)
            {
                FileName fileName;
                scribe.Load(fileName);

                auto script = static_cast<const Script*>(AssetRegistry<ScriptModuleBase>::Find(fileName).Get());
                ATMOS_ASSERT_MESSAGE(script, "The script used by this instance must exist by this point.");
                ATMOS_ASSERT_MESSAGE(script->IsScript(), "The script used by this instance must be a script.");
                impl.reset(new Instance::Impl(*script->impl->module, fileName));
                impl->dependencies = &script->dependencies;
                impl->globalItems = &globalItems;
                InitializeImpl();
            }
        }
    }

    void Script::Instance::InitializeImpl()
    {
        if(impl)
            impl->Init();
    }

    void Script::Instance::SetupForCleanExecution()
    {
        impl->vMachine->reset();
        impl->vMachine->breakRequest(false);
    }

    void Script::Instance::SetupForResumingExecution()
    {
        impl->vMachine->breakRequest(false);
    }

    Script::Instance::Instance() : suspended(false)
    {}

    Script::Instance::Instance(const Script &script) : impl(new Impl(*script.impl->module, script.GetFileName())), suspended(false)
    {
        impl->dependencies = &script.dependencies;
        impl->globalItems = &globalItems;
        InitializeImpl();
    }

    Script::Instance::Instance(const Instance &arg) : impl((arg.impl) ? new Impl(*arg.impl) : nullptr), suspended(false), globalItems(arg.globalItems), executeName(arg.executeName), parameters(arg.parameters)
    {
        InitializeImpl();
    }

    Script::Instance::Instance(Instance &&arg) : impl(std::move(arg.impl)), suspended(std::move(arg.suspended)), caller(std::move(arg.caller))
    {
        InitializeImpl();
    }

    Script::Instance& Script::Instance::operator=(const Instance &arg)
    {
        if (arg.impl)
            impl.reset(new Impl(*arg.impl));
        else
            impl.reset();

        return *this;
    }

    Script::Instance& Script::Instance::operator=(Instance &&arg)
    {
        impl = std::move(arg.impl);
        return *this;
    }

    Script::Instance::~Instance()
    {
        ScriptController::Remove(*this);
    }

    bool Script::Instance::operator==(const Instance &arg) const
    {
        return impl->module == arg.impl->module;
    }

    bool Script::Instance::operator!=(const Instance &arg) const
    {
        return !(*this == arg);
    }

    void Script::Instance::Set()
    {
        impl.reset();
        suspended = false;
    }

    void Script::Instance::Set(const Script &script)
    {
        impl.reset(new Impl(*script.impl->module, script.GetFileName()));
        impl->dependencies = &script.dependencies;
        impl->globalItems = &globalItems;
        suspended = false;
        InitializeImpl();
    }

    bool Script::Instance::IsValid() const
    {
        return impl && impl->isInitialized;
    }

    Script::Instance::operator bool() const
    {
        return IsValid();
    }

    const FileName& Script::Instance::GetFileName() const
    {
        return impl->fileName;
    }

    void Script::Instance::ExecuteDeferred()
    {
        if (!IsValid())
            return;

        SetupForCleanExecution();
        (executeName.empty()) ? ScriptController::Add(*this) : ScriptController::Add(*this, executeName, parameters);
    }

    void Script::Instance::ExecuteDeferred(const SymbolName &overrideExecuteName, const Script::Instance::ItemVector &overrideParameters)
    {
        if (!IsValid())
            return;

        SetupForCleanExecution();
        (executeName.empty()) ? ScriptController::Add(*this) : ScriptController::Add(*this, overrideExecuteName, overrideParameters);
    }

    void Script::Instance::ExecuteImmediately()
    {
        if (!IsValid())
            return;

        SetupForCleanExecution();
        (executeName.empty()) ? ScriptController::AddAndLaunch(*this) : ScriptController::AddAndLaunch(*this, executeName, parameters);
    }

    void Script::Instance::ExecuteImmediately(const SymbolName &overrideExecuteName, const Script::Instance::ItemVector &overrideParameters)
    {
        if (!IsValid())
            return;

        SetupForCleanExecution();
        (executeName.empty()) ? ScriptController::AddAndLaunch(*this) : ScriptController::AddAndLaunch(*this, overrideExecuteName, overrideParameters);
    }

    void Script::Instance::Resume()
    {
        if (!IsValid())
            return;

        SetupForResumingExecution();

        if (!suspended)
            ExecuteDeferred();
        else
        {
            suspended = false;
            (executeName.empty()) ? ScriptController::Add(*this) : ScriptController::Add(*this, executeName, parameters);
        }
    }

    void Script::Instance::Suspend()
    {
        if (!IsValid())
            return;

        impl->vMachine->breakRequest(true);
        suspended = true;
    }

    void Script::Instance::ForceQuit()
    {
        ScriptController::Remove(*this);

        if (!IsValid())
            return;

        impl->vMachine->reset();
        impl->vMachine->breakRequest(true);
    }

    bool Script::Instance::IsExecuting() const
    {
        return ScriptController::IsRunning(*this);
    }

    bool Script::Instance::IsSuspended() const
    {
        return suspended;
    }

    Falcon::VMachineWrapper& Script::Instance::GetVM()
    {
        return impl->vMachine;
    }

    void Script::Instance::AddGlobalItem(const ScriptItem &add)
    {
        globalItems.push_back(add);
    }

    void Script::Instance::RemoveGlobalItem(size_t index)
    {
        globalItems.erase(globalItems.begin() + index);
    }

    void Script::Instance::RemoveGlobalItem(const Name &name)
    {
        for (auto loop = globalItems.begin(); loop != globalItems.end(); ++loop)
        {
            if (loop->name == name)
            {
                globalItems.erase(loop);
                return;
            }
        }
    }

    ScriptItem* Script::Instance::FindGlobalItem(const Name &name)
    {
        for (auto &loop : globalItems)
        {
            if (loop.name == name)
                return &loop;
        }

        return nullptr;
    }

    const ScriptItem* Script::Instance::FindGlobalItem(const Name &name) const
    {
        for (auto &loop : globalItems)
        {
            if (loop.name == name)
                return &loop;
        }

        return nullptr;
    }

    ScriptItem* Script::Instance::FindGlobalItem(size_t index)
    {
        if (globalItems.size() <= index)
            return nullptr;

        return &globalItems[index];
    }

    const ScriptItem* Script::Instance::FindGlobalItem(size_t index) const
    {
        if (globalItems.size() <= index)
            return nullptr;

        return &globalItems[index];
    }

    const Script::Instance::ItemVector& Script::Instance::GetGlobalItems() const
    {
        return globalItems;
    }

    ScriptItem* Script::Instance::SetOrAddGlobalItem(const Name &name, const Variant &variant)
    {
        auto found = FindGlobalItem(name);
        if (found)
        {
            found->value = std::move(variant);
            return found;
        }
        else
        {
            globalItems.push_back(ScriptItem(name, variant));
            return &globalItems.back();
        }
    }

    void Script::Instance::SetExecuteName()
    {
        executeName.clear();
    }

    void Script::Instance::SetExecuteName(const Script::SymbolName &set)
    {
        executeName = set;
    }

    const Script::SymbolName& Script::Instance::GetExecuteName() const
    {
        return executeName;
    }

    void Script::Instance::AddParameter(const ScriptItem &add)
    {
        parameters.push_back(add);
    }

    void Script::Instance::RemoveParameter(size_t index)
    {
        parameters.erase(parameters.begin() + index);
    }

    void Script::Instance::RemoveParameter(const Name &name)
    {
        for (auto loop = parameters.begin(); loop != parameters.end(); ++loop)
        {
            if (loop->name == name)
            {
                parameters.erase(loop);
                return;
            }
        }
    }

    ScriptItem* Script::Instance::FindParameter(const Name &name)
    {
        for (auto &loop : parameters)
        {
            if (loop.name == name)
                return &loop;
        }

        return nullptr;
    }

    const ScriptItem* Script::Instance::FindParameter(const Name &name) const
    {
        for (auto &loop : parameters)
        {
            if (loop.name == name)
                return &loop;
        }

        return nullptr;
    }

    ScriptItem* Script::Instance::FindParameter(size_t index)
    {
        if (parameters.size() <= index)
            return nullptr;

        return &parameters[index];
    }

    const ScriptItem* Script::Instance::FindParameter(size_t index) const
    {
        if (parameters.size() <= index)
            return nullptr;

        return &parameters[index];
    }

    const Script::Instance::ItemVector& Script::Instance::GetParameters() const
    {
        return parameters;
    }

    ScriptCaller& Script::Instance::GetCaller()
    {
        return caller;
    }

    const ScriptCaller& Script::Instance::GetCaller() const
    {
        return caller;
    }

    Script::Script(Dependencies &&dependencies, const FilePath &filePath) : ScriptModuleBase(std::move(dependencies)), impl(new Impl(filePath))
    {}

    Script::Script(Falcon::Module &wrapped, const FileName &fileName) : impl(new Impl(wrapped, fileName))
    {}

    Script::Script(const Script &arg) : ScriptModuleBase(arg), impl(new Impl(*arg.impl))
    {}

    Script::Script(Script &&arg) : ScriptModuleBase(std::move(arg)), impl(std::move(arg.impl))
    {}

    Script& Script::operator=(const Script &arg)
    {
        ScriptModuleBase::operator=(arg);
        impl.reset(new Impl(*arg.impl));
        return *this;
    }

    Script& Script::operator=(Script &&arg)
    {
        impl = std::move(arg.impl);
        return *this;
    }

    bool Script::operator==(const Script &arg) const
    {
        return ScriptModuleBase::operator==(arg);
    }

    bool Script::operator!=(const Script &arg) const
    {
        return !(*this == arg);
    }

    Script::Instance Script::MakeInstance() const
    {
        return Instance(*this);
    }

    const FileName& Script::GetFileName() const
    {
        return impl->fileName;
    }

    Falcon::Module& Script::GetBaseModule()
    {
        return *impl->module;
    }

    bool Script::IsModule() const
    {
        return false;
    }

    bool Script::IsScript() const
    {
        return true;
    }

    INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE(Script)
    {
        ::Inscription::BaseSerialize<ScriptModuleBase>(*table.GetScribe(), *this);
        (*table.GetScribe())(impl->fileName);
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::Script)
    INSCRIPTION_TABLE_END

    INSCRIPTION_INSCRIPTER_DEFINE_SERIALIZE_FUNCTION(::Atmos::Script)
    {
        BaseSerialize<::Atmos::ScriptModuleBase>(scribe, obj);
        INSCRIPTION_SERIALIZE_TABLE;
    }
}

namespace Atmos
{
    bool ScriptCompiler::SetScriptToCompile(const FilePath &filePath)
    {
        if (filePath.IsEmpty())
        {
            OutputNoFilePathCompilationError();
            return false;
        }

        // Make sure that falcon is initialized
        ScriptRegistry::InitializeFalcon();

        try
        {
            inFocus.reset(new ScriptModule(*ScriptLoader::GetModuleLoader().loadFile(filePath.c_str()), filePath));
        }
        catch (::Falcon::Error *err)
        {
            Falcon::AutoCString edesc(err->toString());
            Logger::Log(String("Script compilation has encountered an error.\n") + edesc.c_str(),
                Logger::Type::ERROR_MODERATE,
                Logger::NameValueVector{ NameValuePair("File Path", filePath.GetValue()) });

            return false;
        }

        return true;
    }

    bool ScriptCompiler::SetScriptToCompile(const FilePath &filePath, const ScriptModule::Dependencies &dep)
    {
        if (filePath.IsEmpty())
        {
            OutputNoFilePathCompilationError();
            return false;
        }

        // Make sure that falcon is initialized
        ScriptRegistry::InitializeFalcon();

        try
        {
            inFocus.reset(new ScriptModule(*ScriptLoader::GetModuleLoader().loadFile(filePath.c_str()), filePath, dep));
        }
        catch (::Falcon::Error *err)
        {
            OutputGeneralCompilationError(String(Falcon::AutoCString(err->toString()).c_str()));
            return false;
        }

        return true;
    }

    bool ScriptCompiler::SetScriptToCompile(Script &script)
    {
        // Make sure that falcon is initialized
        ScriptRegistry::InitializeFalcon();

        try
        {
            inFocus.reset(new ScriptModule(script.GetBaseModule(), script.GetFileName()));
        }
        catch (::Falcon::Error *err)
        {
            OutputGeneralCompilationError(String(Falcon::AutoCString(err->toString()).c_str()));
            return false;
        }

        return true;
    }

    bool ScriptCompiler::SetScriptToCompile(Script &script, const ScriptModule::Dependencies &dep)
    {
        // Make sure that falcon is initialized
        ScriptRegistry::InitializeFalcon();

        try
        {
            inFocus.reset(new ScriptModule(script.GetBaseModule(), script.GetFileName(), dep));
        }
        catch (::Falcon::Error *err)
        {
            OutputGeneralCompilationError(String(Falcon::AutoCString(err->toString()).c_str()));
            return false;
        }

        return true;
    }

    void ScriptCompiler::AddDependency(const FileName &fileName)
    {
        if (!inFocus)
            return;

        auto found = AssetRegistry<ScriptModuleBase>::Find(fileName);
        if (!found)
            const_cast<ScriptModuleBase*>(static_cast<const ScriptModuleBase*>(found))->AddDependency(fileName);
    }

    void ScriptCompiler::AddDependency(ScriptModule &module)
    {
        inFocus->AddDependency(module);
    }

    bool ScriptCompiler::Compile()
    {
        if (!inFocus)
            return false;

        try
        {
            Falcon::Runtime runtime(&ScriptLoader::GetModuleLoader());
            SetupRuntimeDependencies(runtime, inFocus->GetDependencies());
            runtime.addModule(&inFocus->GetBaseModule());
        }
        catch (::Falcon::Error *err)
        {
            OutputGeneralCompilationError(String(Falcon::AutoCString(err->toString()).c_str()));
            return false;
        }

        return true;
    }

    void ScriptCompiler::Reset()
    {
        inFocus.reset();
    }

    std::unique_ptr<ScriptModule> ScriptCompiler::inFocus;

    void ScriptCompiler::OutputNoFilePathCompilationError()
    {
        Logger::Log(String("A script with no file path was attempted to be compiled.\n"),
            Logger::Type::ERROR_MODERATE);
    }

    void ScriptCompiler::OutputGeneralCompilationError(const String &err)
    {
        if (inFocus)
        {
            Logger::Log(String("Script compilation has encountered an error.\n") + err,
                Logger::Type::ERROR_MODERATE,
                Logger::NameValueVector{ NameValuePair("File Name", inFocus->GetFileName().GetValue()) });
        }
        else
        {
            Logger::Log(String("Script compilation has encountered an error.\n") + err,
                Logger::Type::ERROR_MODERATE);
        }
    }
}