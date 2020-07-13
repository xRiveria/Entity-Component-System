#include "ECSPrecompiledHeader.h"
#include "Component.h"
#include "System.h"

using namespace EntitySystem;

struct Position : Component<Position>  //Creates a new component of type/family Position. 
{
	Position(float x) : x(x) {};
	float x;
};

struct Wind : public System
{
	Wind()
	{
	
	}
};

int main()
{




	std::cout << "Derp!\n";
	std::cin.get();
}