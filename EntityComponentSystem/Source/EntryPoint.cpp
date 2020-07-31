#include "ECSPrecompiledHeader.h"
#include "Component.h"
#include "System.h"
#include "EntityManager.h"

using namespace EntitySystem;

struct Position : Component<Position>  //Creates a new component of type/family Position. 
{
	Position(float x) : x(x) {};
	float x;
};

class Wind : public System
{
	Wind()
	{
		
	}

	void Update(int deltaTime)
	{
		for (auto& entity : registeredEntities)
		{
			std::cout << "Entity " << entity.entityID << "Update!" << std::endl;
		}
	}
};

int main()
{
	//Create the basic building blocks.
	std::unique_ptr<EntityManager> entityManager = std::make_unique<EntityManager>();

	//Add Systems
	std::unique_ptr<System> windSystem = std::make_unique<Wind>();

	//Initialize Game

	//Add An Entity

	//Run Game for "1 Second at 50 FPS"
	for (int i = 0; i < 50; i++)
	{

	}
}