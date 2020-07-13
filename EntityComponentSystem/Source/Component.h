#pragma once
#include "ECSPrecompiledHeader.h"

//This is the base implementation of a component. Simply inherit from this to create your own custom components. 
namespace EntitySystem
{
	//Each component stores a unique family ID that corresponds to its component type. 
	//All components now inherit from a class which has a static member that is shared across all components. That counter counts up from 0 and each ID belongs to a unique component family.
	//As each new component is called, they are assigned a family by a static line (this will only get run once).

	struct ComponentCounter 
	{
		static int familyCounter;
	};

	template <typename ComponentType>
	struct Component
	{
		static inline int ComponentFamily()   //Stores the family ID for this component in here. Since its static, it will only be initialized once. 
		{
			static int family = ComponentCounter::familyCounter++;
			return family;
		}
	};

	//Utility method for getting a family type.
	template <typename ComponentFamily>
	static int GetComponentFamily()
	{
		return Component<typename std::remove_const<ComponentFamily>::type>::family();
	}
}