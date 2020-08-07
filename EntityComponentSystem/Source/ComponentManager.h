#pragma once
#include "ECSPrecompiledHeader.h"
#include "Entity.h"
#include "EntityMap.h"

/// ==== Component Managers ====

///A component is a chunk of data that is associated with an entity.
///Rather than having entities hold their own components, all components of the same type are handled by a component manager for that type.
///All interactions with the component data thus goes through the component manager.
///In this way, when we run update methods on all components of a type, they are held in contiguous memory.
///Thus, this can lead to a big performance increase on CPU-intensive game components. One important thing to note is that we are going to preallocate this data.
///This means that this contigous array won't change size through the games running, as changing sizes would be an expensive operation that would involve copying a ton of data.
///Thus, if we initialize the array at 1024 spots, it will remain at 1024 forever. This means that we will need to keep track of the current size of the array, since we can't just poll array->size().

///We will also be making these component managers "generic".
///Held within each component manager is an array of data that holds a bunch of structs - the components.
///Since its obvious that all the component managers are going to do essentially the same thing, it follows naturally that we should write one set of code that will correctly handle all components.
///By creating this generic component manager, we can also do some really neat things, such as creating a generic "add component  to entity" method that will be able to figure out which component manager needs to be called.
///We do this by utilizing the power of C++ templates. 

/// ==== Implementation ====

///A component manager needs to be able to add a component, access a component from a specific entity, remove a component from a specific entity and iterate over all its items.
///Our data structure chosen here will be a HashMap (which has an EntityID that points to the actual data for access), with an underlying array of data that can be iterated through.

namespace EntitySystem
{
    //This is our contigous array that will hold all related components.
    //Setting the initial size to 1 will allow us to catch errors more easily, as anything that tries to access data at Index 0 will automatically be a red alert.
    //In this way, Index 0 becomes the equivalant of Index -1 (if we were using ints instead of unsigned ints) - if we’re trying to access it or return it, something probably doesn’t exist or went wrong.
    //We are using arrays instead of lists/vectors here because:
    //std::list is a linked list, and doesn’t really fit our requirements due to both how deletions work (similar to std::vector), and it’s not built for random access by index.
    //Because we have a map on top of the array (referencing indices in the array), we need the map to remain mostly valid through a removal from the array. If we use std::vector and then need to remove our first component (vec.erase(vec.begin())), all our indices shift (vec[2] becomes vec[1], etc.). This invalidates our entire map and would require a full update of the map.

    template <typename ComponentType>
    struct ComponentData
    {
        unsigned int size = 1;
        std::array<ComponentType, MAX_NUMBER_OF_COMPONENTS>* data;
    };

    class BaseComponentManager
    {
    public:
        BaseComponentManager() = default;
        virtual ~BaseComponentManager() = default;
        BaseComponentManager(const BaseComponentManager&) = default;
        BaseComponentManager& operator=(const BaseComponentManager&) = default;
        BaseComponentManager(BaseComponentManager&&) = default;
        BaseComponentManager& operator=(BaseComponentManager&&) = default;
    };

    template <typename ComponentType>
    class ComponentManager : public BaseComponentManager {
    public:
        using LookupType = ComponentType;

        ComponentManager()
        {
            componentData.data = static_cast<std::array<ComponentType, 1024>*>(malloc(sizeof(ComponentType) * 1024));
        }

        //When adding a component, we just need to make sure that both our data structures are correctly updated.
        //We need to add the component to the end of our list, as well as adding a mapping from the Entity to the index in the list.

        ComponentInstance AddComponent(Entity entity, ComponentType& component) 
        {
            ComponentInstance newInstance = componentData.size;   //ComponentInstance maps to an unsigned integer. This creates a new integer that is essentially the size of the current list of components.
            componentData.data->at(newInstance) = component;      //We set the data at the new index to our component.
            entityMap.Add(entity, newInstance);                   //We create a new map that links our entity and the component's index in the list together.
            componentData.size++;                                 //Finally, we increase the size of the component list.
            return newInstance;
        }

        //Removing a component might sound simple, but the implementation is more complex that just array.erase().
        //We have to remember that we're dealing with a static array, and not a vector.
        //Were we to simply erase indices from the array whenever they were removed, we would end up with holes as the engine ran and items were deleted.
        //This not only ruins our idea of a tightly packed array, but will also eventually overflow our array and leave us in the dirt.
        //We solve this by taking the last item in the list and moving it to fill whichever item's spot that was removed.
        //In this way, we're guarenteed to always have tightly packed data, at the cost of the slight overhead of copying a component and updating its index in the hashmap.

        void DestroyComponent(Entity entity)
        {
            //Gets the instance number of the entity in question. 
            ComponentInstance instance = entityMap.GetInstance(entity);

            //Move the last component to the deleted position to maintain data coherence.
            ComponentInstance lastComponent = componentData.size - 1;
            componentData.data[instance] = componentData.data[lastComponent];
            Entity lastEntity = entityMap.GetEntity(lastComponent);

            //Update our map with the changes.
            entityMap.Remove(entity);
            entityMap.Update(lastEntity, instance);

            //Reduces the size of the list now that we have destroyed a component and moved the last item to its position.
            componentData.size--;
        }

        //To access a component from an entity, we first lookup the entity in the hashmap to find where in the array the component is held.
        //Once we have the index, we simply return the component at the index in the array. 

        LookupType* LookupComponent(Entity entity) 
        {
            ComponentInstance instance = entityMap.GetInstance(entity);
            return &componentData.data->at(instance);
        }

    private:
        ComponentData<ComponentType> componentData;
        EntityMap entityMap;
    };
}  