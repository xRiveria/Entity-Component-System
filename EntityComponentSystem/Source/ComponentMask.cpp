#include "ECSPrecompiledHeader.h"
#include "ComponentMask.h"

namespace EntitySystem
{
	bool ComponentMask::IsNewMatch(EntitySystem::ComponentMask oldMask, EntitySystem::ComponentMask systemMask)
	{
		return Matches(systemMask) && !oldMask.Matches(systemMask);
	}

	bool ComponentMask::IsNoLongerMatched(EntitySystem::ComponentMask oldMask, EntitySystem::ComponentMask systemMask)
	{
		return oldMask.Matches(systemMask) && !Matches(systemMask);
	}

	bool ComponentMask::Matches(EntitySystem::ComponentMask systemMask) { return ((mask & systemMask.mask) == systemMask.mask); }
}