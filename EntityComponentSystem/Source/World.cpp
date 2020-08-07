#include "ECSPrecompiledHeader.h"
#include "World.h"
#include "EntityManager.h"
#include "ComponentMask.h"
#include "EntityHandle.h"
#include "System.h"

namespace EntitySystem
{
	World::World(std::unique_ptr<EntityManager> entityManager) : entityManager(std::move(entityManager)) {}

	void World::Initialize()
	{
		for (auto& system : systems)
		{
			system->Initialize();
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

	EntityHandle World::CreateEntity() { return { entityManager->RegisterNewEntity(), this }; }

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

	void World::UpdateEntityMask(EntitySystem::Entity const& entity, EntitySystem::ComponentMask oldMask) {
		ComponentMask newMask = entityMasks[entity];

		for (auto& system : systems) {
			ComponentMask systemSignature = system->GetSignature();
			if (newMask.IsNewMatch(oldMask, systemSignature)) {
				// We match but didn't match before
				system->RegisterEntity(entity);
			}
			else if (newMask.IsNoLongerMatched(oldMask, systemSignature)) {
				system->UnregisterEntity(entity);
			}
		}
	}
}


