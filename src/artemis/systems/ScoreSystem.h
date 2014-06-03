#ifndef SCORE_SYSTEM_H
#define SCORE_SYSTEM_H

#include <artemis/systems/IntervalEntityProcessingSystem.h>
#include <artemis/Aspect.h>
#include <artemis/ComponentMapper.h>
#include "EntitySystemTypes.h"

#include "../components/Components.h"

#include <string>

namespace artemis
{

class EntityScoreSystem: public IntervalEntityProcessingSystem
{
private:
    ComponentMapper<ScoreComponent, ctScore> scoreMapper;
protected:
public:
    EntityScoreSystem()
        :IntervalEntityProcessingSystem(Aspect::getAspectForAll(ctScore), 1.0f/3.f, estScore)
    {
        registerMapper(&scoreMapper);
    }

    virtual void process(Entity* e) override
    {
        ScoreComponent* sc = scoreMapper.get(e);

        sc->text->setString(std::to_string(sc->score));
    }
};

};

#endif // SCORE_SYSTEM_H
