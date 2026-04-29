#include "Component.h"
class Actor;

Component::Component(Actor* owner, int updateOrder) :owner(owner) {}