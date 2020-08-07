#pragma once
#include <ECSPrecompiledHeader.h>
#include "System.h"
#include "Component.h"
#include "Entity.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "ComponentMask.h"
#include <map>
#include "ComponentHandle.h"

///How do systems find out that a component has been added or removed?
///There are too many interconnected pieces around our component system, and thus the World will be in charge of linking all of them together.
///While we could essentially put all kinds of functional managing in the EntityManager, it would be going way past its scope of what we wanted it to simply be.
///Suddenly, it would have to deal with all systems at runtime, calling Update(), handling parallelization etc.
///For this reason, we turn to the "World". It acts as a glue between the parts above.
///The "World" is essentially running the show. It holds a reference to all of the aforementioned parts of the engine: all the systems, all the component managers, and the entity manager. Access to any of these is run through the world, but the world doesn’t actually do any of the work itself, just hands out references and makes calls.
///So, the world keeps track of all the systems, and systems have a pointer back to the world they exist in. All calls to the rest of the engine from the systems go through the world.

namespace EntitySystem 
{
    struct EntityHandle;
    class System;

    class World 
    {
    public:
        explicit World(std::unique_ptr<EntityManager> entityManager);

        //As we talked about in our discussion about Systems, we have an “Initialize” method that is called after the basic game initialization happens (systems & first components have been added), but before the game begins to run. 
        //Calling Initialize() on the world will be forwarded to all its systems.

        void Initialize();
        void Update(int dt);
        void Render();
        
        //For CreateEntity(), the World communicates with the EntityManager to create a new entity. We then wrap the entity in a pretty handle.
        EntityHandle CreateEntity();
        void AddSystem(std::unique_ptr<System> system);
        void DestroyEntity(Entity entity);

        //All component adding and removal will be done through the world. This is because there are actually two things we need to worry about when adding a component:
        //Allocating/deallocating the required space in the component manager.
        //Notifying systems that a component has been added/removed.

        template <typename ComponentType>
        void AddCustomComponentManager(std::unique_ptr<ComponentManager<ComponentType>> manager) {
            int family = GetComponentFamily<ComponentType>();
            if (family >= componentManagers.size()) {
                componentManagers.resize(family + 1);
            }
            componentManagers[family] = manager;
        }

        template <typename ComponentType>
        void AddComponent(Entity const& entity, ComponentType&& component) 
        {
            ComponentManager<ComponentType>* manager = GetComponentManager<ComponentType>();
            manager->AddComponent(entity, component);

            ComponentMask oldMask = entityMasks[entity];
            entityMasks[entity].AddComponent<ComponentType>();

            UpdateEntityMask(entity, oldMask);
        }

        template <typename ComponentType>
        void RemoveComponent(Entity const& entity) 
        {
            ComponentManager<ComponentType>* manager = GetComponentManager<ComponentType>();
            ComponentHandle<ComponentType> component = manager->LookupComponent(entity);
            component.Destroy();

            ComponentMask oldMask = entityMasks[entity];
            entityMasks[entity].RemoveComponent<ComponentType>();

            UpdateEntityMask(entity, oldMask);
        }

        //Unpack is one of the utility methods that we will use the most when working with our engine. 
        //Unpack gives us a pretty interface to get a bunch of components from an entity. For example, let’s say we have a system that wants the Transform, Motion, and Health component for Entity 3. 
        //Instead of needing references to all 3 of those component managers, we simply do the following:

        template <typename ComponentType, typename... Args>
        void Unpack(Entity e, ComponentHandle<ComponentType>& handle, ComponentHandle<Args>&... args) {
            typedef ComponentManager<ComponentType> ComponentManagerType;
            auto mgr = GetComponentManager<ComponentType>();
            handle = ComponentHandle<ComponentType>(e, mgr->LookupComponent(e), mgr);

            // Recurse
            Unpack<Args...>(e, args...);
        }

        // Base case
        template <typename ComponentType>
        void Unpack(Entity e, ComponentHandle<ComponentType>& handle) {
            typedef ComponentManager<ComponentType> ComponentManagerType;
            auto mgr = GetComponentManager<ComponentType>();
            handle = ComponentHandle<ComponentType>(e, mgr->LookupComponent(e), mgr);
        }
        
    private:
        std::unique_ptr<EntityManager> entityManager;
        std::vector<std::unique_ptr<System>> systems;
        std::vector<std::unique_ptr<BaseComponentManager>> componentManagers;
        std::map<Entity, ComponentMask> entityMasks;

        void UpdateEntityMask(Entity const& entity, ComponentMask oldMask);

        template <typename ComponentType>
        ComponentManager<ComponentType>* GetComponentManager() 
        {
            int family = GetComponentFamily<ComponentType>();

            if (family >= componentManagers.size()) {
                componentManagers.resize(family + 1);
            }

            if (!componentManagers[family]) {
                componentManagers[family] = std::make_unique<ComponentManager<ComponentType>>();
            }

            return static_cast<ComponentManager<ComponentType>*>(componentManagers[family].get());
        }
    };
}