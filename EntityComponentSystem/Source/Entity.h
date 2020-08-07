#pragma once

namespace EntitySystem
{
	struct Entity
	{
		unsigned int entityID;
		friend bool operator<(const Entity& l, const Entity& r) { return l.entityID < r.entityID; }
	};
}