#include "ECSPrecompiledHeader.h"
#include "Component.h"
#include "System.h"
#include "EntityManager.h"
#include "ComponentHandle.h"
#include "World.h"
#include "ComponentManager.h"
#include "EntityHandle.h"

/* Useful Links
- http://gamadu.com/artemis/
*/

///==== Entity Component System ====

///In this Entity Component System, we're going to have Entities, Components and Systems.
///Entities are assigned components based on what attributes they have.
///This is the very essence of ECS: an entity is just a collection of components that provide functionality.
///When done properly, components can be added and removed to add or remove functionality. 
///Both entities and components have no functionality, they're simply bags of data.
///Adding a new feature or functionality is simple - simply add components and systems as needed. 
///Because systems are independent and only deal with a specific subset of components, the game engine has very low coupling, which makes it a lot easier to debug and plan.
///In addition, the majority of systems don’t actually need to be run in a certain order, so we can have different systems execute on different threads concurrently, significantly boosting our performance.

///==== ComponentManagers ====

///Entities and Components are tied together by ComponentManagers.
///ComponentManagers manage all components of one type and keep references to which entities own them.
///Giving components their own managers as opposed to letting entities own components may seem to be an arbitrary decision, but doing so actually gives a serious performance increase.
///The most important information to know here is that processors love to iterate over arrays of contiguous data. The less we jump around the computer’s memory, the better.
///Let’s consider that a game might have hundreds of components of a given type. The performance difference between jumping around in memory to update their maximum health and simply running through an array is sizable.

///==== Systems ====

///Now that we have covered Entities and Components, how do we actually add functionality to them?
///The answer is this: Systems. Entities and Components are just data containers, and systems are the ones who actually modify that data.
///In our implementation, a system can specify a set of component types that it wishes to pay attention to. Any entity that has the necessary components will be updated by the system.
///For example, a Movement System will pay attention to any entity with the "Transform" and "Motion" components. As components are added and removed, the list of entities that the movement system pays attention to will change.
///Of course, during every update loop, the movement system will serve to move any entities in its list. 
///The most important thing to note is that a system only pays attention to whichever entities has the components it is focused on, and ignores the remaining entities.

using namespace EntitySystem;

struct Position : Component<Position>  //Creates a new component of type/family Position. 
{
	Position(float x) : x(x) {};
	float x;
};

class Wind : public System
{
public:
	Wind()
	{
		signature.AddComponent<Position>();
	}

	void Update(int deltaTime)
	{
		for (auto& entity : registeredEntities)
		{
			ComponentHandle<Position> position;
			parentWorld->Unpack(entity, position);

			//Move every 1 second.
			position->x += 1.0f * (deltaTime / 1000.0f);

			//Print entity position.
			std::cout << "Entity " << entity.entityID << position->x << std::endl;
		}
	}
};


int main()
{
	//Create the basic building blocks.
	std::unique_ptr<EntityManager> entityManager = std::make_unique<EntityManager>();
	std::unique_ptr<World> world = std::make_unique<World>(std::move(entityManager)); //std::move is used to indicate that an object t may be "moved from", i.e. allowing the efficient transfer of resources from t to another object.
	//Add Systems															 
	std::unique_ptr<System> wind = std::make_unique<Wind>();
	world->AddSystem(std::move(wind));


	//Initialize Game
	world->Initialize();

	//Add An Entity
	auto tumbleweed = world->CreateEntity();
	tumbleweed.AddComponent(Position(0));


	//Run Game for "1 Second at 50 FPS"
	for (int i = 0; i < 50; i++)
	{
		world->Update(20);
	}
}