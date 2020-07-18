// Bunch of testing
//

#include "ent.h"
#include <thread>

using namespace std;

struct Position
{
	float x, y;

	Position(float x, float y) :
		x(x), y(y)
	{
	}

	~Position()
	{

	}
};

struct Direction
{
	float x, y;

	Direction(float x, float y) :
		x(x), y(y)
	{
	}

	~Direction()
	{
		std::cout << "Direction Destructor" << std::endl;
	}
};

int main()
{
	Hx::World w(64);
	Hx::Entity e = w.CreateEntity();
	Hx::Entity e1 = w.CreateEntity();
	Hx::Entity e2 = w.CreateEntity();
	
	e.AddComponent<Position>(2.0f, 3.0f);
	e.AddComponent<Direction>(2.0f, 3.0f);

	e1.AddComponent<Position>(2.0f, 3.0f);
	e1.AddComponent<Direction>(2.0f, 3.0f);

	e2.AddComponent<Position>(2.0f, 3.0f);
	e2.AddComponent<Direction>(2.0f, 3.0f);

	Position pos = e2.GetComponent<Position>();

	std::cout << pos.x << ", " << pos.y << std::endl;

	w.DestroyEntity(e);

	e = w.CreateEntity();

	e.AddComponent<Position>(4.0f, 5.0f);
	e.AddComponent<Direction>(7.0f, 6.0f);

	w.DestroyEntity(e);
	w.DestroyEntity(e1);
	w.DestroyEntity(e2);

	return 0;
}
