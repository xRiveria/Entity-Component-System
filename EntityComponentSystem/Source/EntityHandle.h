#pragma once
#include "ECSPrecompiledHeader.h"
#include "Entity.h"
#include "World.h"
#include "ComponentHandle.h"

/// ==== Entity Handles ====

///Because of how entity component systems are designed, a lot of classic reasoning is actually backwards when referring to entities and components, particularly when it comes to ownership.
///Thinking tranditionally, a component is owned by an entity. Let's say that my player is wearing armor - an Armor Component.
///The way that we think of ownership is that the player "owns" the suit of armor. 

///However, since we know the architecture works in the background, we know that really, components are owned by their component managers - "they own themselves".
///Practically, this means that our code looks more like this: armorComponentManager.addComponent(playerEntity, armorComponent), than playerEntity.AddComponent(armorComponent).
///While we could try to make all of the users of our game engine learn this new (counterintuitive) concept of ownership, realistically it doesn’t make that much sense.
///Entity Component systems are already hard to reason about because they are a new paradigm that does away with the typical object oriented approach — why make it more complicated by enforcing backwards logic just because that’s how the back-end is implemented?
///Thus, we are going to make some handles that will allow us to get as close as possible to making syntaxes easy to work with and understand, while stripping away a lot of the underlying code to achieve something like "playerEntity.AddComponent(armorComponent)".

namespace EntitySystem
{
    struct EntityHandle
    {
        //Data.
        Entity entity;
        World* world;

        //Helper Methods.
        void Destroy()
        {
            world->DestroyEntity(entity);
        }

        template<typename ComponentType>
        void AddComponent(ComponentType&& component)
        {
            world->AddComponent<ComponentType>(entity, std::forward<ComponentType>(component));
        }

        template<typename ComponentType>
        void RemoveComponent()
        {
            world->RemoveComponent<ComponentType>(entity);
        }

        template<typename ComponentType>
        ComponentHandle<ComponentType> GetComponent()
        {
            ComponentHandle<ComponentType> handle;
            world->Unpack(entity, handle);
            return handle;
        }
    };
}