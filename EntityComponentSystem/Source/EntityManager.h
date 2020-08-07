#pragma once
#include "Entity.h"

///==== Entity Managers ====

///The entity manager handles which IDs are handed out to new entities.
///When you create a new entity, the entity manager will take care of making sure that no other living entities share the same ID.
///For this reason, the entity manager itself will be a singleton. 

namespace EntitySystem
{
	class EntityManager
	{
	public:
		Entity RegisterNewEntity();
		void RemoveEntity(Entity entity);

	private:
		unsigned int lastEntity = 0;
	};
}
