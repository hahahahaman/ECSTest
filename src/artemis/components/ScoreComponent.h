#ifndef SCORE_COMPONENT_H
#define SCORE_COMPONENT_H

#include <string>

#include <SFML/Graphics/Text.hpp>

#include "ComponentTypes.h"

namespace artemis
{

class ScoreComponent: public Component
{
public:
    ScoreComponent()
        :Component(ctScore), score(0), text(nullptr)
    {
    }

    int score;
    sf::Text* text;

};

};


#endif // SCORE_COMPONENT_H
