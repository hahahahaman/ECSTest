#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include <cstdlib>

#include <artemis/Aspect.h>
#include <artemis/systems/EntityProcessingSystem.h>
#include <artemis/ComponentMapper.h>
#include <artemis/utils/Bag.h>

#include "EntitySystemTypes.h"
#include "../components/Components.h"
#include "../../Collision.h"
#include "../../FPS.hpp"

namespace artemis
{

/**
* Change in movement and collision handling.
*/
class EntityCollisionSystem : public EntityProcessingSystem
{
private:
    ComponentMapper<CollisionComponent, ctCollision> colMapper;
    ComponentMapper<MovementComponent, ctMovement> moveMapper;
    ComponentMapper<PositionComponent, ctPosition> posMapper;
    ComponentMapper<ScoreComponent, ctScore> scoreMapper;
    //possibly need other mappers later to resolve collisions

    sf::Clock ballRespawnTimer;
protected:

    bool posValid(Entity* e, sf::Vector2f pos)
    {
        CollisionComponent* cc = colMapper.get(e);
        MovementComponent* mc = moveMapper.get(e);
        PositionComponent* pc = posMapper.get(e);

        const Bag<Entity *> * actives = this->getActives();

        bool Return = true;

        for(unsigned int i = 0; i < actives->size(); ++i)
        {
            CollisionComponent* cc_other = colMapper.get(actives->get(i));
            PositionComponent* pc_other = posMapper.get(actives->get(i));

            // If CollisionComponent pointers are the same move on.
            if(cc_other == cc) continue;

            // Place the FloatRects in the correct position.
            sf::FloatRect rect(cc->collisionObject->getLocalBounds());
            rect.left += pos.x;
            rect.top += pos.y;

            sf::FloatRect rect_other(cc_other->collisionObject->getLocalBounds());
            rect_other.left += pc_other->pos.x;
            rect_other.top += pc_other->pos.y;

            if(ye::isRectIntersecting(rect, rect_other))
            {
                //fprintf(stdout, "*** Intersecting ***\n");

                // Place the collisionObjects in the correct position.
                sf::ConvexShape shape = *(cc->collisionObject);
                shape.setPosition(pos);

                sf::ConvexShape shape_other = *(cc_other->collisionObject);
                shape_other.setPosition(pc_other->pos);

                ye::Simplex simplex;
                if(ye::isIntersecting(shape, shape_other, simplex))
                {
                    // Resolve the collision.

                    //ball collision
                    if(cc->collisionFlags[CollisionComponent::Ball])
                    {
                        //ball to wall
                        if(cc_other->collisionFlags[CollisionComponent::Wall])
                        {
                            if(pos.x != pc->pos.x)
                            {
                                //mc->speed.x = mc->acceleration.x = 0.f;
                                mc->speed.x = -mc->speed.x;
                                mc->acceleration.x = -mc->acceleration.x;
                            }
                            if(pos.y != pc->pos.y)
                            {
                                //mc->speed.y = mc->acceleration.y = 0.f;
                                mc->speed.y = -mc->speed.y;
                                mc->acceleration.y = -mc->acceleration.y;
                            }
                        }

                        //ball to paddle
                        if(cc_other->collisionFlags[CollisionComponent::Paddle])
                        {
                            if(pos.x != pc->pos.x)
                            {
                                /*
                                if(mc->speed.x < 0)
                                    mc->speed.x -= 1.f;
                                else
                                    mc->speed.x += 1.f;
                                */
                                float centerPaddle = rect_other.top + rect_other.width/2.f;

                                //float absSpeed = fabs(mc->speed.y);
                                float diffPos = pos.y - centerPaddle;
                                //printf("%f\n", diffPos);
                                mc->speed.y = diffPos * 10.f;
                                mc->acceleration.y = diffPos;


                                mc->speed.x = -mc->speed.x;
                                mc->acceleration.x = -mc->acceleration.x;
                            }
                            else if(pos.y != pc->pos.y)
                            {


                                //Get absolute value of y speed
                                //Get distance away from center
                                //change speed so that it's angled
                                //if hits above the center then the speed is negative
                                //if hits below center then the speed is positive

                                mc->speed.y = -mc->speed.y;
                                mc->acceleration.y = -mc->acceleration.y;
                                /*
                                //hits bottom of paddle
                                if(diffPos > 0)
                                {

                                }
                                //if ball is above middle of paddle
                                if(pos.y < centerPaddle)
                                {


                                    //going down
                                    if(mc->speed.y > 0)
                                    {
                                        mc->speed.y = -mc->speed.y;
                                        mc->acceleration.y = -mc->acceleration.y;
                                    }
                                }
                                else if(pos.y > centerPaddle)
                                {
                                    if(mc->speed.y < 0)
                                    {
                                        mc->speed.y = -mc->speed.y;
                                        mc->acceleration.y = -mc->acceleration.y;
                                    }
                                }
                                else
                                {
                                    mc->speed.y = 0;
                                    mc->acceleration.y = 0;
                                }
                                */
                            }

                            //cc_other->collisionObject->setFillColor(sf::Color::Green);
                        }

                        //ball to goal
                        if(cc_other->collisionFlags[CollisionComponent::Goal])
                        {
                            ballRespawnTimer.restart();

                            if(rand()%2)
                            {
                                mc->speed.x = 100.f;
                                mc->acceleration.x = 50.f;
                            }
                            else
                            {
                                mc->speed.x = -100.f;
                                mc->acceleration.x = -50.f;
                            }


                            if(rand()%2)
                            {
                                mc->speed.y = rand()%100 + 50;
                                mc->acceleration.y = 50;
                            }
                            else
                            {
                                mc->speed.y = -(rand()%100 + 50);
                                mc->acceleration.y = -50.f;
                            }

                            if(scoreMapper.has(actives->get(i)))
                            {
                                ScoreComponent* sc = scoreMapper.get(actives->get(i));
                                ++sc->score;
                            }
                        }
                    }

                    //paddle to wall
                    if(cc->collisionFlags[CollisionComponent::Paddle])
                    {
                        if(pos.x != pc->pos.x)
                        {
                            mc->speed.x = mc->acceleration.x = 0.f;
                        }
                        if(pos.y != pc->pos.y)
                        {
                            mc->speed.y = mc->acceleration.y = 0.f;
                        }
                    }

                    //mc->speed = -mc->speed;
                    //mc->acceleration = -mc->acceleration;

                    // New position is invalid.
                    Return = false;
                }
            }
        }

        return Return;
    }

public:
    EntityCollisionSystem()
        :EntityProcessingSystem(Aspect::getAspectForAll(ctCollision, ctPosition), estCollision)
    {
        registerMapper(&colMapper);
        registerMapper(&moveMapper);
        registerMapper(&posMapper);
        registerMapper(&scoreMapper);
    }

    virtual void process(Entity* e) override
    {
        CollisionComponent* cc = colMapper.get(e);
        PositionComponent* pc = posMapper.get(e);
        if(ballRespawnTimer.getElapsedTime().asSeconds() <= 1.0f)
        {
            if(cc->collisionFlags[CollisionComponent::Ball])
            {
                pc->pos = sf::Vector2f(400.f, 300.f);
            }
        }

        if(moveMapper.has(e))
        {
            MovementComponent* mc = moveMapper.get(e);
            PositionComponent* pc = posMapper.get(e);

            sf::Vector2f movement(mc->speed), posIncrement(0.f, 0.0f);

            movement *= FPS::FPSControl.GetSpeedFactor();

            if(movement.x != 0)
            {
                if(movement.x >= 0)     posIncrement.x =  FPS::FPSControl.GetSpeedFactor();
                else                    posIncrement.x = -FPS::FPSControl.GetSpeedFactor();
            }

            if(movement.y != 0)
            {
                if(movement.y >= 0)     posIncrement.y =  FPS::FPSControl.GetSpeedFactor();
                else                    posIncrement.y = -FPS::FPSControl.GetSpeedFactor();
            }

            while(true)
            {
                if(posValid(e, sf::Vector2f(pc->pos.x + posIncrement.x, pc->pos.y)))
                {
                    pc->pos.x += posIncrement.x;
                }
                else
                    posIncrement.x = 0.f;

                if(posValid(e, sf::Vector2f(pc->pos.x, pc->pos.y + posIncrement.y)))
                {
                    pc->pos.y += posIncrement.y;
                }
                else
                    posIncrement.y = 0.f;

                //the total movement is decremented
                movement -= posIncrement;

                if(posIncrement.x > 0 && movement.x <= 0) posIncrement.x = 0;
                if(posIncrement.x < 0 && movement.x >= 0) posIncrement.x = 0;

                if(posIncrement.y > 0 && movement.y <= 0) posIncrement.y = 0;
                if(posIncrement.y < 0 && movement.y >= 0) posIncrement.y = 0;

                if(movement.x == 0) posIncrement.x = 0;
                if(movement.y == 0) posIncrement.y = 0;

                if(movement.x == 0 && movement.y == 0)             break;
                if(posIncrement.x == 0 && posIncrement.y == 0)     break;
            }
        }
    }
};

};

#endif // COLLISION_SYSTEM_H
