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
};

struct Direction
{
	float x, y;

	Direction(float x, float y) :
		x(x), y(y)
	{
	}
};

struct Renderable
{

};

int main()
{
	Hx::World w(64);
	Hx::Entity e = w.CreateEntity();
	Hx::Entity e1 = w.CreateEntity();
	Hx::Entity e2 = w.CreateEntity();
	Hx::EntityQueryView q = w.QueryWithComponents<Position, Direction>();
	
	e.AddComponent<Position>(4.0f, 9.0f);
	e.AddComponent<Direction>(7.0f, 3.0f);

	e1.AddComponent<Position>(4.0f, 4.0f);
	e1.AddComponent<Direction>(7.0f, 3.0f);

	e2.AddComponent<Position>(5.0f, 2.0f);
	e2.AddComponent<Direction>(6.0f, 8.0f);

	Hx::Entity myEntity = w.CreateEntity();
	
	myEntity.AddComponent<Position>(69.f, 69.f)
			.AddComponent<Direction>(1.f, 0.f)
			.AddComponent<Renderable>();

	q.Each([](Hx::Entity& e, Position& pos, Direction& dir) {
			//pos.x += dir.x;
			//pos.y += dir.y;

			std::cout << "Position" << pos.x << ", " << pos.y << std::endl;
			std::cout << "Direction" << dir.x << ", " << dir.y << std::endl;
		});

	w.DestroyEntity(e);
	w.DestroyEntity(e1);
	w.DestroyEntity(e2);
	w.DestroyEntity(myEntity);

	return 0;
}
