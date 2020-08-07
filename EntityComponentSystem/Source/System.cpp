#include "ECSPrecompiledHeader.h"
#include "System.h"

namespace EntitySystem
{
	void System::RegisterWorld(World* world)
	{
		parentWorld = world;
	}

	void System::RegisterEntity(const Entity& entity)
	{
		registeredEntities.push_back(entity);
	}

	void System::UnregisterEntity(const Entity& entity)
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

	ComponentMask System::GetSignature() { return signature; }
}

