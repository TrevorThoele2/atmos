#pragma once

#include "Event.h"
#include "Input.h"
#include "ObjectTypeGraph.h"
#include "Camera.h"
#include "Render.h"
#include "nModulatorLinkMap.h"
#include "FilePath.h"
#include "AssetPackage.h"
#include "WorldManager.h"

namespace Atmos
{
    class GameEnvironment
    {
    public:
        static NullEvent eventFrameStart;
        static NullEvent eventFrameStop;
    public:
        static GameEnvironment& Instance();

        static void Init();
        static void Loop();
        static void StopAll();

        static void LoadWorld(const FilePath &path);
        static void LoadStasis(const FilePath &path);

        static ObjectManager MakeObjectManager();
        static ObjectManager& GetGlobalObjectManager();
        static ObjectTypeGraph GetLocalObjectTypeGraph();
        static ObjectTypeGraph GetGlobalObjectTypeGraph();
        static void RegisterAllObjectTypesToSerialization();
        template<class T>
        static void RegisterLocalObjectType();
        template<class T>
        static void RegisterLocalObjectSystem();
        template<class T>
        static void RegisterGlobalObjectType();
        template<class T>
        static void RegisterGlobalObjectSystem();

        static WorldManager& GetWorldManager();

        static Camera& GetCamera();

        static Modulator::LinkMap& GetModulatorLinkMap();

        static AssetPackage& GetAssetPackage();
    private:
        GameEnvironment() = default;

        WorldManager worldManager;

        std::unique_ptr<ObjectManager> globalObjectManager;

        Camera camera;

        RenderHandler renderer;

        Modulator::LinkMap modulatorLinkMap;

        AssetPackage assetPackage;

        static void OnInputPressed(const Input::Key &args);
        static void OnFocusLost();
        static void OnFocusRegain();

        static void Work();
    private:
        static void RegisterAllKnownObjectTypes();

        class ObjectTypeRegistratorBase
        {
        public:
            virtual ~ObjectTypeRegistratorBase() = 0;

            virtual void Register(ObjectManager& manager) = 0;
            virtual void ToTypeGraph(ObjectTypeGraph& typeGraph) = 0;
        };

        template<class T>
        class ObjectTypeRegistrator : public ObjectTypeRegistratorBase
        {
        public:
            void Register(ObjectManager& manager) override;
            void ToTypeGraph(ObjectTypeGraph& typeGraph) override;
        };

        template<class T>
        class ObjectSystemTypeRegistrator : public ObjectTypeRegistratorBase
        {
        public:
            void Register(ObjectManager& manager) override;
            void ToTypeGraph(ObjectTypeGraph& typeGraph) override;
        };

        typedef std::unique_ptr<ObjectTypeRegistratorBase> ObjectTypeRegistratorPtr;
        typedef std::vector<ObjectTypeRegistratorPtr> ObjectTypeRegistratorList;

        ObjectTypeRegistratorList localTypes;
        ObjectTypeRegistratorList globalTypes;

        static void PushAllObjectTypes(ObjectTypeRegistratorList& from, ObjectManager& to);
    };

    template<class T>
    void GameEnvironment::RegisterLocalObjectType()
    {
        Instance().localTypes.push_back(std::make_unique<ObjectTypeRegistrator<T>>());
    }

    template<class T>
    void GameEnvironment::RegisterLocalObjectSystem()
    {
        Instance().localTypes.push_back(std::make_unique<ObjectSystemTypeRegistrator<T>>());
    }

    template<class T>
    void GameEnvironment::RegisterGlobalObjectType()
    {
        Instance().globalTypes.push_back(std::make_unique<ObjectTypeRegistrator<T>>());
    }

    template<class T>
    void GameEnvironment::RegisterGlobalObjectSystem()
    {
        Instance().globalTypes.push_back(std::make_unique<ObjectSystemTypeRegistrator<T>>());
    }

    template<class T>
    void GameEnvironment::ObjectTypeRegistrator<T>::Register(ObjectManager& manager)
    {
        manager.RegisterObjectType<T>();
    }

    template<class T>
    void GameEnvironment::ObjectTypeRegistrator<T>::ToTypeGraph(ObjectTypeGraph& typeGraph)
    {
        typeGraph.AddDescription(TypeDescriptionFor<T>());
    }

    template<class T>
    void GameEnvironment::ObjectSystemTypeRegistrator<T>::Register(ObjectManager& manager)
    {
        manager.RegisterObjectSystem<T>();
    }

    template<class T>
    void GameEnvironment::ObjectSystemTypeRegistrator<T>::ToTypeGraph(ObjectTypeGraph& typeGraph)
    {}

    ObjectManager* GetGlobalObjectManager();
}