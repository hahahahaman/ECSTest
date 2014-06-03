#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

#include <artemis/Component.h>
#include <SFML/System.hpp>
#include "ComponentTypes.h"

/**
* Position components.
*/

namespace artemis
{

class PositionComponent : public Component
{
public:
    PositionComponent()
        :Component(ctPosition)
    {
        pos.x = pos.y = 0.f;
    }

    ~PositionComponent() {};

    sf::Vector2f pos;
};

};
#endif // POSITION_COMPONENT_H
