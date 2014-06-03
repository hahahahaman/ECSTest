#ifndef MOVEMENT_COMPONENT_H
#define MOVEMENT_COMPONENT_H

#include <bitset>

#include <SFML/System.hpp>

#include <artemis/Component.h>

#include "ComponentTypes.h"

namespace artemis
{
class MovementComponent : public Component
{
public:

    enum MovementFlag
    {
        Gravity = 0,
    };

    MovementComponent()
        :Component(ctMovement)
    {
        speed.x = speed.y = acceleration.x = acceleration.y = maxSpeed.x = maxSpeed.y = maxAcceleration.x = maxAcceleration.y = 0.f;
    }

    sf::Vector2f speed, acceleration, maxSpeed, maxAcceleration;

    std::bitset<8> movementFlags;
};
};
#endif // MOVEMENT_COMPONENT_H
