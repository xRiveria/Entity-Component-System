#pragma once
#include "Entity.h"

namespace EntityManager
{
	// The entity manager exists to keep track of entity IDs to make sure that conflicts don't happen.
	class EntityManager
	{
	public:
		Entity RegisterNewEntity();
		void RemoveEntity(Entity entity);

	private:
		unsigned int lastEntity = 0;
	};
}
