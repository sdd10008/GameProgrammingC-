#pragma 
#include <vector>
#include "Game.h"
class Component;

class Actor
{
public:
	// state of the actor
	enum State {
		EActive,
		EPaused,
		EDead
	};

	// constructor/destructor
	Actor(class Game* game);
	virtual ~Actor();

	// process actor update
	void Update(float deltaTime);
	// process componet update
	void UpdateComponent(float deltaTime);
	// process derived class update
	virtual void UpdateActor(float deltaTime);
	State getState() { return state; };

	// add component to game class
	void AddComponent(class Component* component);
	// remove component to game class
	void RemoveComponent(class Component* component);

private:
	// variable
	State state;
	Vector2 mPosition;
	float mScale;
	float mRotation;
	std::vector<class Component*> mComponents;
	class Game* game;
};

