#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <vector>
#include <algorithm>

Actor::Actor(Game* game) : game(game),
state(EActive),
mPosition(Vector2{ 0.0f,0.0f }),
mScale(1.0f),
mRotation(0.0f) {
}

Actor::~Actor() {
    while (!mComponents.empty())
    {
        delete mComponents.back();
    }
}

void Actor::Update(float deltaTime) {

}

void Actor::UpdateComponent(float deltaTime) {

}

void Actor::AddComponent(Component* component) {
    mComponents.emplace_back(component);
}

void Actor::RemoveComponent(Component* componet) {
    mComponents.erase(std::remove(mComponents.begin(), mComponents.end(), componet), mComponents.end());
}