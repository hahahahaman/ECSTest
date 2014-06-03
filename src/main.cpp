/**
* main.cpp
*/

#include <iostream>
#include <cstdlib>
#include <memory>
#include <cassert>
#include <cstdlib>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <artemis/World.h>

#include <LTBL/Light/LightSystem.h>

#include "artemis/systems/Systems.h"
#include "artemis/components/Components.h"

#include "FPS.hpp"
#include "TextureManager.hpp"

int main()
{
    srand(time(NULL));

    const int wHeight = 600;
    const int wWidth = 800;
    // initialize
    sf::RenderWindow renderWindow(sf::VideoMode(wWidth,wHeight, 64), "ECSTest");
    renderWindow.setVerticalSyncEnabled(false);
    std::unique_ptr<artemis::World> world(new artemis::World);

    world->setSystem(new artemis::EntityRenderSystem);
    world->setSystem(new artemis::EntityMovementSystem);
    world->setSystem(new artemis::EntityCollisionSystem);
    world->setSystem(new artemis::EntityScoreSystem);

    world->initialize();

    sf::Sprite bg;
    sf::Texture* bgTexture = ye::gTextureManager.getResource("data/background.png");
    bgTexture->setRepeated(true);
    bg.setTexture(*bgTexture);
    bg.setTextureRect(sf::IntRect(0,0, wWidth, wHeight));

    sf::Font airone;
    assert(airone.loadFromFile("data/airone.ttf"));


    // Light system
    ltbl::LightSystem ls(AABB(sf::Vector2f(0.0f, 0.0f),
                              sf::Vector2f(renderWindow.getSize().x, renderWindow.getSize().y)),
                         &renderWindow,
                         "data/lightFin.png",
                         "data/shaders/lightAttenuationShader.frag");

    ls.setView(renderWindow.getView());
    //ls.m_ambientColor = sf::Color(255.0f, 255.0f, 0.0f ,0.1f);
    ls.m_useBloom = true; //TODO: toggling use bloom doesn't work
    ls.m_checkForHullIntersect = false;
    ls.m_obliqueShadows = true;

    // Create a light
    ltbl::Light_Point * testLight = new ltbl::Light_Point();

    //intensity only works with glEnable(GL_BLEND) since it uses glColor4f(r,g,b,a)
    //Attenuation shader handles like all this stuff so intensity in the renderLightSolid func. doesn't do anything
    testLight->setIntensity(1.5f); //affects whether or not the light is drawn, if it is <= 1 it will not draw
    testLight->setColor(sf::Vector3f(1.f, 1.f, 1.f));
    testLight->setCenter(sf::Vector2f(wWidth/2.f, wHeight/2.f));

    testLight->setRadius(450.0f); //increases the size of the texture and the light radius
    testLight->setSize(1.0f); //affects shadow fins and hull intersection
    testLight->setSpreadAngle(360.f);
    testLight->setSoftSpreadAngle(0.0f);
    testLight->setBleed(0.2f); //this is how large the center light circle
    testLight->setLinearizeFactor(0.5f); //how oblique the light is
    testLight->calculateAABB();
    ls.addLight(testLight);

    sf::Vector2f paddleDims(50.f, 100.f);

    /*
        ltbl::ConvexHull* convexHulls[2];

        for(int i = 0; i< 2; i++)
        {
            convexHulls[i] = new ltbl::ConvexHull;

            if(!convexHulls[i]->loadShape("data/testShape.txt"))
                abort();

            // Pre-calculate certain aspects
            convexHulls[i]->calculateNormals();
            convexHulls[i]->calculateAABB();
            convexHulls[i]->setWorldCenter(sf::Vector2f(0.0f, 0.0f));
            convexHulls[i]->setTransparency(0.0f);
            //convexHulls[i]->setOutlineColor(sf::Color::Black);
            //convexHulls[i]->setOutlineThickness(10.f);
            convexHulls[i]->setFillColor(sf::Color(255.f, 255.f, 0.f, 255.f));
            convexHulls[i]->setRenderLightOverHull(false); //so important
            ls.addConvexHull(convexHulls[i]);
        }
    */

    /**
    * Two paddles and the ball, Left paddle, Right paddle, Ball
    */
    artemis::Entity* pong[3];
    artemis::PositionComponent* pongPos[3];
    artemis::RenderComponent* pongRender[3];
    artemis::CollisionComponent* pongColObj[3];
    artemis::MovementComponent* pongMove[3];
    sf::ConvexShape pongColShape[3];
    for(int i = 0; i < 3; ++i)
    {
        pong[i] = world->createEntity();

        pongPos[i] = new artemis::PositionComponent;
        pongRender[i] = new artemis::RenderComponent;
        pongColObj[i] = new artemis::CollisionComponent;
        pongMove[i] = new artemis::MovementComponent;

        //link up collision objects
        pongColObj[i]->collisionObject = &pongColShape[i];

        //link up render
        pongRender[i]->renderTarget = static_cast<sf::RenderTarget*>(&renderWindow);
        pongRender[i]->renderDrawable = static_cast<sf::Drawable*>(pongColObj[i]->collisionObject);
        pongRender[i]->visible = true;

        pong[i]->addComponent(pongPos[i]);
        pong[i]->addComponent(pongRender[i]);
        pong[i]->addComponent(pongColObj[i]);
        pong[i]->addComponent(pongMove[i]);

        pong[i]->addToWorld();
    }
    pongRender[2]->visible = false;

    //pong positions
    pongPos[0]->pos = sf::Vector2f(50.f, wHeight/2.f);
    pongPos[1]->pos = sf::Vector2f(wWidth - 100.f, wHeight/2.f);
    pongPos[2]->pos = sf::Vector2f(wWidth/2.f, wHeight/2.f);

    //pong Collision Objects: left, right, ball
    pongColShape[0].setPointCount(4);
    pongColShape[0].setPoint(0, sf::Vector2f(0.f, 0.f));
    pongColShape[0].setPoint(1, sf::Vector2f(0.f, paddleDims.y));
    pongColShape[0].setPoint(2, paddleDims);
    pongColShape[0].setPoint(3, sf::Vector2f(paddleDims.x, 0.f));
    pongColShape[0].setFillColor(sf::Color::Blue);
    //pongColShape[0].setTexture(ye::gTextureManager.getResource("data/emissive.png"));

    pongColShape[1].setPointCount(4);
    pongColShape[1] = pongColShape[0];
    pongColShape[1].setFillColor(sf::Color::Magenta);

    float ballDim = 40.f;
    pongColShape[2].setPointCount(4);
    pongColShape[2].setPoint(0, sf::Vector2f(-ballDim/2.f, 0.f));
    pongColShape[2].setPoint(1, sf::Vector2f(-ballDim/2.f, ballDim));
    pongColShape[2].setPoint(2, sf::Vector2f(ballDim/2.f, ballDim));
    pongColShape[2].setPoint(3, sf::Vector2f(ballDim/2.f, 0.f));
    pongColShape[2].setFillColor(sf::Color::White);

    //distinguish collision flags
    pongColObj[0]->collisionFlags[artemis::CollisionComponent::Paddle] = 1;
    pongColObj[1]->collisionFlags[artemis::CollisionComponent::Paddle] = 1;
    pongColObj[2]->collisionFlags[artemis::CollisionComponent::Ball] = 1;

    //pong movement
    pongMove[0]->maxSpeed.y = pongMove[1]->maxSpeed.y = 1000.f;
    pongMove[0]->maxAcceleration.y = pongMove[1]->maxAcceleration.y = 100.f;

    pongMove[2]->maxAcceleration = pongMove[2]->maxSpeed = sf::Vector2f(10000.f, 10000.f);

    if(rand()%2)
    {
        pongMove[2]->speed.x = 100.f;
        pongMove[2]->acceleration.x = 50.f;
    }
    else
    {
        pongMove[2]->speed.x = -100.f;
        pongMove[2]->acceleration.x = -50.f;
    }

    if(rand()%2)
    {
        pongMove[2]->speed.y = rand()%100;
        pongMove[2]->acceleration.y = 50;
    }
    else
    {
        pongMove[2]->speed.y = -(rand()%100);
        pongMove[2]->speed.y = -50.f;
    }

    /**
    * Going to do this counter-clockwise: top, left, bottom, right.
    */
    artemis::Entity* walls[4];
    artemis::PositionComponent* wallPos[4];
    artemis::CollisionComponent* wallColObj[4];
    sf::ConvexShape wallColShape[4];

    for(int i = 0; i < 4; ++i)
    {
        //initialize/create stuff
        walls[i] = world->createEntity();
        wallPos[i] = new artemis::PositionComponent;
        wallColObj[i] = new artemis::CollisionComponent;
        wallColShape[i] = sf::ConvexShape(3);

        //set the collisionObject pointer to address of ze collision Objects
        wallColObj[i]->collisionObject = &wallColShape[i];
        wallColObj[i]->collisionFlags[artemis::CollisionComponent::Wall] = 1; //it's a wall

        //add components
        walls[i]->addComponent(wallPos[i]);
        walls[i]->addComponent(wallColObj[i]);

        walls[i]->addToWorld();
    }

    //Useful
    sf::Vector2f topLeft(0.f, 0.f), topRight(wWidth, 0.f), bottomRight(wWidth, wHeight), bottomLeft(0.f, wHeight);

    //initialize position values
    wallPos[0]->pos = topLeft;
    wallPos[1]->pos = topLeft;
    wallPos[2]->pos = bottomLeft;
    wallPos[3]->pos = topRight;

    //initialize wallColShape values

    //top
    wallColShape[0].setPoint(0, topLeft);
    wallColShape[0].setPoint(1, topRight);
    wallColShape[0].setPoint(2, sf::Vector2f(0.0f,-1.f));

    //left
    wallColShape[1].setPoint(0, topLeft);
    wallColShape[1].setPoint(1, bottomLeft);
    wallColShape[1].setPoint(2, sf::Vector2f(-1.f, 0.f));

    wallColObj[1]->collisionFlags[artemis::CollisionComponent::Goal] = 1;

    //bottom
    wallColShape[2] = wallColShape[0];
    wallColShape[2].setPoint(2, sf::Vector2f(0.f, 1.f));

    //right
    wallColShape[3] = wallColShape[1];
    wallColShape[3].setPoint(2, sf::Vector2f(1.f, 0.f));

    wallColObj[3]->collisionFlags[artemis::CollisionComponent::Goal] = 1;

    //scores: right, left
    //artemis::Entity* e_scores[2];
    artemis::ScoreComponent* scores[2];
    //artemis::RenderComponent* scoresRender[2];
    //artemis::PositionComponent* scoresPos[2];
    sf::Text scoreText[2];

    for(int i = 0; i < 2; i++)
    {
        scoreText[i].setFont(airone);
        //e_scores[i] = world->createEntity();

        scores[i] = new artemis::ScoreComponent;
        scores[i]->text = &scoreText[i];
        scores[i]->text->setCharacterSize(50);
        scores[i]->text->setColor(sf::Color::White);

        /*
        scoresRender[i] = new artemis::RenderComponent;
        scoresRender[i]->renderTarget = &renderWindow;
        scoresRender[i]->renderDrawable = static_cast<sf::Drawable*>(scores[i]->text);

        scoresPos[i] = new artemis::PositionComponent;

        e_scores[i]->addComponent(scores[i]);
        e_scores[i]->addComponent(scoresRender[i]);
        e_scores[i]->addComponent(scoresPos[i]);

        e_scores[i]->addToWorld();
        */
    }
    //set Positions
    scores[0]->text->setPosition(wWidth*0.75f, 10.f);
    scores[1]->text->setPosition(wWidth*0.25f, 10.f);

    //Add scores to wall
    walls[1]->addComponent(scores[0]);
    walls[3]->addComponent(scores[1]);

    /*
        sf::Sprite sp;
        sp.setTexture(*ye::gTextureManager.getResource("data/back3.png"));
        sp.setScale(0.1, 0.1);
        */

    float aiTimer = 0.f;
    while (renderWindow.isOpen())
    {
        sf::Event event;

        while(renderWindow.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                renderWindow.close();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            pongMove[1]->speed.y += -50;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            pongMove[1]->speed.y += 50;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            pongMove[0]->speed.y += -50;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            pongMove[0]->speed.y += 50;
        }

        //ai
        {
            aiTimer += FPS::FPSControl.GetSpeedFactor();
            if(aiTimer > 1.f/180.f)
            {
                aiTimer = 0.f;

                if(pongPos[2]->pos.y > pongPos[0]->pos.y + paddleDims.y/2.f )
                {
                    pongMove[0]->speed.y += 30;
                    //pongPos[0]->pos.y += 5;
                }
                else if(pongPos[2]->pos.y < pongPos[0]->pos.y + paddleDims.y/2.f )
                {
                    pongMove[0]->speed.y -= 30;
                    //pongPos[0]->pos.y -= 5;
                }
                else
                {
                  //  pongMove[0]->speed.y = 0;
                }
            }

        }


        renderWindow.clear();

        FPS::FPSControl.OnLoop();

        world->setDelta(FPS::FPSControl.GetSpeedFactor());

        renderWindow.draw(bg);
        world->process();

        renderWindow.draw(scoreText[0]);
        renderWindow.draw(scoreText[1]);

        testLight->setCenter(sf::Vector2f(pongPos[2]->pos.x, pongPos[2]->pos.y + ballDim/2.f));
        // convexHulls[0]->setWorldCenter(pongPos[0]->pos);
        // convexHulls[1]->setWorldCenter(pongPos[1]->pos);

        /*
                sp.setPosition(pongPos[0]->pos);

                renderWindow.draw(sp);
        */

        // calculate the lights
        ls.renderLights();

        // Draw the lights
        ls.renderLightTexture();

        renderWindow.display();
    }

    return 0;
}
