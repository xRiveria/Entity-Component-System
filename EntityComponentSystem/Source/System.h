#pragma once
#include "ECSPrecompiledHeader.h"
#include "Entity.h"
#include "World.h"

namespace EntitySystem
{
	class System
	{
	public:
		System() = default; 
		virtual ~System() = default;
		System(const System&) = default;
		System &operator=(const System&) = default;
		System(System&&) = default;
		System& operator=(System&&) = default;

		virtual void Init() {};
		virtual void Update(int deltaTime) {};
		virtual void Render() {};

		//When a system is added to the world, the world will register itself.
		void RegisterWorld(World* world);

		//When a component is added such that this system should begin acting on it, register will be called.
		void RegisterEntity(const Entity& entity);
		
		//If a component is removed from an entity such that the system should stop acting on it, unregister will be called.
		void UnregisterEntity(const Entity& entity);

	protected:
		std::vector<Entity> registeredEntities;
		World* parentWorld;
		//Component Mask
	};
}
