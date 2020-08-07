#pragma once
#include "ECSPrecompiledHeader.h"
#include "Entity.h"
#include "World.h"
#include "ComponentMask.h"

///==== System ====
///A system is a self contained unit of game functionality. Generally, systems run their logic on every game update.
///Each system specifies which components it wants to pay attention to, and is handed entities based on this.

///Example: I have a system that needs to heal all entities with “Health” components by 1 every 20 seconds. In this situation, the system would specify that it wants to pay attention to “Health” components. 
///Everytime an entity gets a health component, it would be added to the list of entities the system needs to update. Every time an entity loses a health component, the system would stop updating it.

namespace EntitySystem
{
	class World;
	class System
	{
	public:
		System() = default;
		virtual ~System() = default;
		System(const System&) = default;
		System& operator=(const System&) = default;
		System(System&&) = default;
		System& operator=(System&&) = default;

		//The Initialize method is expected to be called *before* update() or render() are run, but *after* the basic game initialization has happened (all systems have been added, all components have been added, etc.). 
		virtual void Initialize() {};

		//These two both start getting called* after* init() gets run.
		//Check https://gafferongames.com/post/fix_your_timestep/ for more information on the difference between the two.
		virtual void Update(int deltaTime) {};
		virtual void Render() {};

		//When a system is added to the world, the world will register itself.
		void RegisterWorld(World* world);

		//When a component is added such that this system should begin acting on it, register will be called.
		void RegisterEntity(const Entity& entity);
		
		//If a component is removed from an entity such that the system should stop acting on it, unregister will be called.
		void UnregisterEntity(const Entity& entity);

		ComponentMask GetSignature();

	protected:
		std::vector<Entity> registeredEntities;
		World* parentWorld;
		ComponentMask signature;
	};
}
