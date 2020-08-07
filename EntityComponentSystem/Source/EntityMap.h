#pragma once
#include <ECSPrecompiledHeader.h>
#include <map>
#include "Entity.h"

/*
 * Effectively a bidirectional map
 * Entity <-> ComponentInstance
 */

namespace EntitySystem
{
    const int MAX_NUMBER_OF_COMPONENTS = 1024;
    using ComponentInstance = unsigned int; 

    struct EntityMap
    {
        Entity GetEntity(ComponentInstance instance) { return instanceToEntity.at(instance); }
        ComponentInstance GetInstance(Entity entity) { return entityToInstance.at(entity); }
   
        void Add(Entity entity, ComponentInstance instance)
        {
            entityToInstance.insert({ entity, instance });
            instanceToEntity.at(instance) = entity;
        }

        void Update(Entity entity, ComponentInstance instance)
        {
            entityToInstance.at(entity) = instance;
            instanceToEntity.at(instance) = entity; 
        }

        void Remove(Entity entity) { entityToInstance.erase(entity); }

        std::map<Entity, ComponentInstance> entityToInstance;
        std::array<Entity, MAX_NUMBER_OF_COMPONENTS> instanceToEntity;
    };
}
