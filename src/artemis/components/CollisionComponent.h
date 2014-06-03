#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include <bitset>

#include <artemis/Component.h>

#include "ComponentTypes.h"

#include "../../Collision.h"

namespace artemis
{

class CollisionComponent : public Component
{
public:
    enum CollisionFlag
    {
        Ghost = 0,
        MapOnly,
        Wall,
        Paddle,
        Ball,
        Goal
    };

    CollisionComponent()
        : Component(ctCollision), collisionObject(nullptr)
    {
    }

    sf::ConvexShape* collisionObject;

    std::bitset<16> collisionFlags;
};

};

#endif // COLLISION_COMPONENT_H
