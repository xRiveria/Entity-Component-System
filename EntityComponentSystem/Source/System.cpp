#include "ECSPrecompiledHeader.h"
#include "System.h"

void EntitySystem::System::RegisterEntity(const Entity& entity)
{
	registeredEntities.push_back(entity);
}

void EntitySystem::System::Unregisterentity(const Entity& entity)
{
	for (auto it = registeredEntities.begin(); it != registeredEntities.end(); ++it)
	{
		Entity entity = *it;
		if (entity.entityID == entity.entityID)
		{
			registeredEntities.erase(it);
			return;
		}
	}
}
