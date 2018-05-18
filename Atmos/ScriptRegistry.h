#pragma once

#include <memory>
#include <vector>

#include "Script.h"
#include "AssetRegistry.h"

namespace Atmos
{
    // Holds script modules and scripts
    template<>
    class AssetRegistry<ScriptModuleBase> : public AssetRegistryBase<ScriptModuleBase, std::unique_ptr<ScriptModuleBase>, FileName, AssetRegistry<ScriptModuleBase>>
    {
    public:
        typedef std::vector<ScriptModuleBase*> Vector;
    public:
        // Returns true if the module was unique
        // The module returned is not guaranteed to be the type requested
        // If catch exception is false any exception related to creating the script module will not be caught
        static std::pair<bool, ReferenceT> RegisterModule(const FilePath &filePath, bool catchException = true);
        // Returns true if the script was unique
        // The module returned is not guaranteed to be the type requested
        // If catch exception is false any exception related to creating the script module will not be caught
        static std::pair<bool, ReferenceT> RegisterScript(Script::Dependencies &&dependencies, const FilePath &filePath, bool catchException = true);
        // Returns true if the module/ID was unique
        // The module returned is not guaranteed to be the type requested
        // If catch exception is false any exception related to creating the script module will not be caught
        static std::pair<bool, ReferenceT> RegisterModule(ID id, const FilePath &filePath, bool catchException = true);
        // Returns true if the script/ID was unique
        // The module returned is not guaranteed to be the type requested
        // If catch exception is false any exception related to creating the script module will not be caught
        static std::pair<bool, ReferenceT> RegisterScript(ID id, Script::Dependencies &&dependencies, const FilePath &filePath, bool catchException = true);

        static ReferenceT Find(const KeyT &key);
        static ReferenceT Find(ID id);

        static Vector GetScripts();
        static Vector GetModules();
    private:
        typedef std::unique_ptr<ScriptModuleBase> ScriptModuleBasePtr;

        AssetRegistry();
        AssetRegistry(const AssetRegistry &arg) = delete;
        AssetRegistry& operator=(const AssetRegistry &arg) = delete;

        template<class T, class... Args>
        static std::pair<bool, ScriptModuleBase*> RegisterCommon(const FileName &name, bool catchException, Args && ... args);
        template<class T, class... Args>
        static std::pair<bool, ScriptModuleBase*> RegisterCommon(ID id, const FileName &name, bool catchException, Args && ... args);

        static AssetT& UnpackStore(StoreT &unpack);
        static const AssetT& UnpackStore(const StoreT &unpack);
    private:
        typedef AssetRegistryBase<ScriptModuleBase, std::unique_ptr<ScriptModuleBase>, FileName, AssetRegistry<ScriptModuleBase>> BaseT;
        friend AssetRegistryBase<ScriptModuleBase, std::unique_ptr<ScriptModuleBase>, FileName, AssetRegistry<ScriptModuleBase>>;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };

    template<class T, class... Args>
    std::pair<bool, ScriptModuleBase*> AssetRegistry<ScriptModuleBase>::RegisterCommon(const FileName &name, bool catchException, Args && ... args)
    {
        typedef std::pair<bool, ScriptModuleBase*> RetType;

        StoreT *made;

        // Try making the module
        if (!catchException)
            made = Emplace(name, ScriptModuleBasePtr(new T(std::forward<Args>(args)...)));
        else
        {
            try
            {
                made = Emplace(name, ScriptModuleBasePtr(new T(std::forward<Args>(args)...)));
            }
            catch (Falcon::Error*)
            {
                return RetType(false, nullptr);
            }
        }

        return RetType(true, made->get());
    }

    template<class T, class... Args>
    static std::pair<bool, ScriptModuleBase*> AssetRegistry<ScriptModuleBase>::RegisterCommon(ID id, const FileName &name, bool catchException, Args && ... args)
    {
        typedef std::pair<bool, ScriptModuleBase*> RetType;

        StoreT *made;

        // Try making the module
        if (!catchException)
            made = Emplace(id, name, ScriptModuleBasePtr(new T(std::forward<Args>(args)...)));
        else
        {
            try
            {
                made = Emplace(id, name, ScriptModuleBasePtr(new T(std::forward<Args>(args)...)));
            }
            catch (Falcon::Error*)
            {
                return RetType(false, nullptr);
            }
        }

        return RetType(true, made->get());
    }

    typedef AssetRegistry<ScriptModuleBase> ScriptRegistry;
}