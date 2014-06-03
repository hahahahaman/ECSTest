#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "EntitySystemTypes.h"

#include "../../FPS.hpp"

#include "../components/MovementComponent.h"
#include "../components/PositionComponent.h"

#include <artemis/systems/EntityProcessingSystem.h>
#include <artemis/ComponentMapper.h>

namespace artemis
{

/**
* Change in MovementComponent handled in MovementSystem.
*/
class EntityMovementSystem : public EntityProcessingSystem
{
private:
    ComponentMapper<MovementComponent, ctMovement> moveMapper;
    ComponentMapper<PositionComponent, ctPosition> posMapper;
protected:

public:
    EntityMovementSystem()
        :EntityProcessingSystem(Aspect::getAspectForAll(ctMovement, ctPosition), estMovement)
    {
        registerMapper(&moveMapper);
        registerMapper(&posMapper);
    }

    virtual void process(Entity* e) override
    {
        MovementComponent* mc = moveMapper.get(e);
        //PositionComponent* pc = posMapper.get(e);

        if(mc->movementFlags[MovementComponent::Gravity])
            mc->acceleration.y += 10.f;

        if(mc->acceleration.x > mc->maxAcceleration.x)  mc->acceleration.x =  mc->maxAcceleration.x;
        if(mc->acceleration.x < -mc->maxAcceleration.x) mc->acceleration.x = -mc->maxAcceleration.x;
        if(mc->acceleration.y > mc->maxAcceleration.y)  mc->acceleration.y =  mc->maxAcceleration.y;
        if(mc->acceleration.y < -mc->maxAcceleration.y) mc->acceleration.y = -mc->maxAcceleration.y;

        mc->speed.x += mc->acceleration.x * FPS::FPSControl.GetSpeedFactor();
        mc->speed.y += mc->acceleration.y * FPS::FPSControl.GetSpeedFactor();

        if(mc->speed.x > mc->maxSpeed.x)  mc->speed.x =  mc->maxSpeed.x;
        if(mc->speed.x < -mc->maxSpeed.x) mc->speed.x = -mc->maxSpeed.x;
        if(mc->speed.y > mc->maxSpeed.y)  mc->speed.y =  mc->maxSpeed.y;
        if(mc->speed.y < -mc->maxSpeed.y) mc->speed.y = -mc->maxSpeed.y;
    }
};

};


#endif // MOVEMENT_SYSTEM_H
