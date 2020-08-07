#pragma once
#include "Entity.h"
#include "ComponentManager.h"

///Similar to EntityHandles, our ComponentHandles will allow us to abstract away underlying code that will make things easier for users to understand.

namespace EntitySystem
{
    template<typename ComponentType>
    struct ComponentHandle
    {
        using ExposedComponentType = typename ComponentManager<ComponentType>::LookupType;

        Entity owner;
        ExposedComponentType* component;
        ComponentManager<ComponentType>* manager;

        //Empty Constructor used for World::Unpack().
        ComponentHandle() {};
        ComponentHandle(Entity owner, ExposedComponentType* component, ComponentManager<ComponentType>* manager)
        {
            this->owner = owner;
            this->component = component;
            this->manager = manager;
        }

        //Handle->Member is the same as handle.component->member.
        ExposedComponentType* operator->() const { return component; }
        void Destroy() 
        {
            manager->DestroyComponent(owner); 
        }
    };
}