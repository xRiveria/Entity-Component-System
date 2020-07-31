#pragma once
#include "ECSPrecompiledHeader.h"
#include "Component.h"
#include "Entity.h"
#include "EntityManager.h"
#include "System.h"

namespace EntitySystem
{
	struct EntityHandle;

	class World
	{
	public:
		explicit World(std::unique_ptr<EntityManager> entityManager);
		//Should be called before the first update, but after instantiation.
		void Initialize();

		//Update game logic. This is unrelated to a frame
		//gafferongames.com/post/fix_your_timestep
		void Update(int deltaTime)
		{
			//Render a frame.
		}
		void Render();
		EntityHandle createEntity();
		void AddSystem(std::unique_ptr<System> system);
		void DestroyEntity(Entity entity);

	private:
		std::unique_ptr<EntityManager> entityManager;
		std::vector<std::unique_ptr<System>> systems;
		//std::vector<std::unique_ptr<BaseComponentSystem>> componentManagers;
		
		void UpdateEntityMask(Entity const* entity, ComponentMask oldMask);
	};



}
