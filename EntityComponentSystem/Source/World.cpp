#include "ECSPrecompiledHeader.h"
#include "World.h"
#include "EntityManager.h"

namespace EntitySystem
{
	World::World(std::unique_ptr<EntityManager> entityManager) : entityManager(std::move(entityManager)) {}

	void World::Initialize()
	{
		for (auto& system : systems)
		{
			system->Init();
		}
	}

	void World::Update(int deltaTime)
	{
		for (auto& system : systems)
		{
			system->Update(deltaTime);
		}
	}

	void World::Render()
	{
		for (auto& system : systems)
		{
			system->Render();
		}
	}

	//EntityHandle World::createEntity() { return entityManager->}
	void World::DestroyEntity(EntitySystem::Entity entity)
	{
		for (auto& system : systems)
		{
			system->UnregisterEntity(entity);
		}

		entityManager->RemoveEntity(entity);
	}

	void World::AddSystem(std::unique_ptr<EntitySystem::System> system)
	{
		system->RegisterWorld(this);
		systems.push_back(std::move(system));
	}

	//void World::UpdateEntityMask()
	//{
//
	//}


}