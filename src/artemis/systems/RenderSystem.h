#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <artemis/Aspect.h>
#include <artemis/systems/IntervalEntityProcessingSystem.h>
#include <artemis/ComponentMapper.h>
#include "EntitySystemTypes.h"
#include "../components/RenderComponent.h"
#include "../components/PositionComponent.h"

namespace artemis
{

const float SPF = 1.f/60.f; //seconds per frame

class EntityRenderSystem : public IntervalEntityProcessingSystem
{
private:
    //some mapper
    ComponentMapper<RenderComponent, ctRender> renderMapper;
    ComponentMapper<PositionComponent, ctPosition> positionMapper;
protected:
public:
    EntityRenderSystem()
        :IntervalEntityProcessingSystem(Aspect::getAspectForAll(ctRender,ctPosition), SPF, estRender)
    {
        registerMapper(&renderMapper);
        registerMapper(&positionMapper);
    }

    ~EntityRenderSystem()
    {
    }

    virtual void process(artemis::Entity* e) override
    {
        RenderComponent* rc = renderMapper.get(e);
        PositionComponent* pc = positionMapper.get(e);

        if(!rc->visible) return;

        sf::Transform tempTransform;
        tempTransform.translate(pc->pos);
        rc->renderTarget->draw(*rc->renderDrawable, sf::RenderStates(tempTransform));

    }
};

};
#endif // RENDER_SYSTEM_H
