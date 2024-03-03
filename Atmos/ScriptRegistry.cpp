
#include "ScriptRegistry.h"

#include "ScriptLoader.h"
#include "Logger.h"
#include "AssetPackage.h"

#include <falcon/engine.h>

namespace Atmos
{
    void AssetRegistry<ScriptModuleBase>::InitializeFalcon()
    {
        Instance();
    }

    std::pair<bool, AssetRegistry<ScriptModuleBase>::ReferenceT> AssetRegistry<ScriptModuleBase>::RegisterModule(const FilePath &filePath, bool catchException)
    {
        return RegisterModule(AssetT::nullID, filePath, catchException);
    }

    std::pair<bool, AssetRegistry<ScriptModuleBase>::ReferenceT> AssetRegistry<ScriptModuleBase>::RegisterScript(Script::Dependencies &&dependencies, const FilePath &filePath, bool catchException)
    {
        return RegisterScript(AssetT::nullID, std::move(dependencies), filePath, catchException);
    }

    std::pair<bool, AssetRegistry<ScriptModuleBase>::ReferenceT> AssetRegistry<ScriptModuleBase>::RegisterModule(ID id, const FilePath &filePath, bool catchException)
    {
        typedef std::pair<bool, ReferenceT> RetType;

        // Figure out if this file path is unique
        FileName name(filePath);
        ScriptModuleBase::SetupModuleExtension(name);
        {
            auto found = map.find(name);
            if (map.find(name) != map.end())
                return RetType(false, ReferenceT(found->second.get()));
        }

        // Find out if the ID is unique
        {
            auto found = idMap.find(id);
            if (found != idMap.end())
                return RetType(false, ReferenceT(found->second->second.get()));
        }

        return RegisterCommon<ScriptModule>(id, name, catchException, *ScriptLoader::GetModuleLoader().loadFile(filePath.c_str()), name);
    }

    std::pair<bool, AssetRegistry<ScriptModuleBase>::ReferenceT> AssetRegistry<ScriptModuleBase>::RegisterScript(ID id, Script::Dependencies &&dependencies, const FilePath &filePath, bool catchException)
    {
        typedef std::pair<bool, ReferenceT> RetType;

        // Figure out if this file path is unique
        FileName name(filePath);
        ScriptModuleBase::SetupModuleExtension(name);
        {
            auto found = map.find(name);
            if (map.find(name) != map.end())
                return RetType(false, ReferenceT(found->second.get()));
        }

        FilePath usePath(filePath);
        usePath.SetName(name);

        return RegisterCommon<Script>(id, name, catchException, std::move(dependencies), usePath);
    }

    AssetRegistry<ScriptModuleBase>::ReferenceT AssetRegistry<ScriptModuleBase>::Find(const KeyT &key)
    {
        FileName name(key);
        ScriptModuleBase::SetupModuleExtension(name);
        return FindImpl(name);
    }

    AssetRegistry<ScriptModuleBase>::ReferenceT AssetRegistry<ScriptModuleBase>::Find(ID id)
    {
        return FindImpl(id);
    }

    AssetRegistry<ScriptModuleBase>::Vector AssetRegistry<ScriptModuleBase>::GetScripts()
    {
        Vector ret;
        for (auto &loop : map)
        {
            if (loop.second->IsScript())
                ret.push_back(loop.second.get());
        }

        return ret;
    }

    AssetRegistry<ScriptModuleBase>::Vector AssetRegistry<ScriptModuleBase>::GetModules()
    {
        Vector ret;
        for (auto &loop : map)
        {
            if (loop.second->IsModule())
                ret.push_back(loop.second.get());
        }

        return ret;
    }

    AssetRegistry<ScriptModuleBase>::AssetRegistry()
    {
        Falcon::Engine::Init();
    }

    typename AssetRegistry<ScriptModuleBase>::AssetT& AssetRegistry<ScriptModuleBase>::UnpackStore(StoreT &unpack)
    {
        return *unpack;
    }

    const AssetRegistry<ScriptModuleBase>::AssetT& AssetRegistry<ScriptModuleBase>::UnpackStore(const StoreT &unpack)
    {
        return *unpack;
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(AssetRegistry<ScriptModuleBase>)
    {
        ::inscription::TrackingChangerStack tracking(scribe, false);

        if (scribe.IsOutput())
        {
            // SAVING
            inscription::ContainerSize size(map.size());
            scribe.Save(size);

            for (auto &loop : map)
            {
                // True if the base was a script
                scribe.Save(loop.second->IsScript());
                // Save name and ID
                scribe.Save(inscription::RemoveConst(loop.second->GetFileName()));
                scribe.Save(loop.second->GetID());

                // Dependencies
                auto &dependencies = loop.second->GetDependencies();
                inscription::ContainerSize dependencySize(dependencies.size());
                scribe.Save(dependencySize);
                for (auto &dependencyLoop : dependencies)
                    scribe.Save(dependencyLoop->GetID());
            }
        }
        else
        {
            // LOADING
            // Retrieve IDs -> module names
            inscription::ContainerSize size;
            scribe.Load(size);

            struct Hold
            {
                bool wasScript;
                std::vector<ScriptModuleBase::ID> dependencies;
                FileName fileName;
                ScriptModuleBase *wrapped;

                Hold(bool wasScript, const FileName &fileName) : wasScript(wasScript), fileName(fileName), wrapped(nullptr) {}
            };

            std::vector<Script*> scripts;
            std::vector<Hold> hold;
            hold.reserve(size.Get());

            // Create all of the modules and hold their dependencies as IDs in a vector
            {
                Map::iterator itr;
                auto loop = size;
                while (loop-- > 0)
                {
                    bool wasScript;
                    scribe.Load(wasScript);

                    FileName fileName;
                    scribe.Load(fileName);

                    ScriptModuleBase::ID id = 0;
                    scribe.Load(id);

                    // Create the module
                    {
                        auto buffer = AssetPackage::RetrieveScript(fileName);
                        ATMOS_ASSERT_MESSAGE(buffer, "This buffer must exist.");

                        // Check that the file actually has data
                        if (buffer->second == 0)
                            Logger::Log("A script file has no data.",
                                Logger::Type::ERROR_SEVERE,
                                Logger::NameValueVector{ NameValuePair( "File Name", fileName.GetValue()) });

                        // Try loading the buffer
                        try
                        {
                            Falcon::ROStringStream stream(static_cast<char*>(buffer->first), buffer->second);
                            Falcon::TranscoderByte transcoder(&stream);

                            auto module = ScriptLoader::GetModuleLoader().loadModule(&transcoder);
                            module->name(fileName.GetWithoutExtension().c_str());
                            module->path(fileName.GetWithoutExtension().c_str());

                            if (wasScript)
                            {
                                hold.push_back(Hold(true, fileName));
                                itr = map.emplace(fileName, new Script(*module, fileName)).first;
                                SetID(*itr->second, id);
                                hold.back().wrapped = itr->second.get();
                                scripts.push_back(static_cast<Script*>(hold.back().wrapped));
                            }
                            else
                            {
                                hold.push_back(Hold(false, fileName));
                                itr = map.emplace(fileName, new ScriptModule(*module, fileName)).first;
                                SetID(*itr->second, id);
                                hold.back().wrapped = itr->second.get();
                            }

                            idMap.emplace(id, itr);
                        }
                        catch (Falcon::Error *err)
                        {
                            Falcon::AutoCString edesc(err->toString());
                            Logger::Log(String("Loading a script file encountered a problem.\n") + edesc.c_str(),
                                Logger::Type::ERROR_SEVERE,
                                Logger::NameValueVector{ NameValuePair("File Name", fileName.GetValue()) });
                        }
                        catch (...)
                        {
                            auto e = std::current_exception();

                            // Catch any loading errors
                            Logger::Log("The script file " + fileName.GetValue() + " was not loadable. It is probably corrupt or invalid.",
                                Logger::Type::ERROR_SEVERE,
                                Logger::NameValueVector{ NameValuePair("File Name", fileName.GetValue()) });
                        }
                    }

                    auto &held = hold.back();

                    inscription::ContainerSize dependencySize;
                    scribe.Load(dependencySize);
                    held.dependencies.reserve(dependencySize.Get());

                    while (dependencySize-- > 0)
                    {
                        ScriptModuleBase::ID dependencyID = 0;
                        scribe.Load(dependencyID);

                        held.dependencies.push_back(dependencyID);
                    }
                }
            }

            // Setup all dependencies
            for (auto &loop : hold)
            {
                // Setup dependencies
                for (auto &dependencyLoop : loop.dependencies)
                    loop.wrapped->AddDependency(*static_cast<ScriptModule*>(idMap.find(dependencyLoop)->second->second.get()));
            }
        }
    }
}