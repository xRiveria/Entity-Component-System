#include "ECSPrecompiledHeader.h"
#include "EntityManager.h"

namespace EntitySystem
{
	Entity EntityManager::RegisterNewEntity()
	{
		lastEntity++;
		return { lastEntity }; //Initializer.
	}

	void EntityManager::RemoveEntity(Entity entity)
	{

	}
}