#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include <artemis/Component.h>
#include <SFML/Graphics.hpp>

#include "ComponentTypes.h"

/*
what to render? render like a sprite on a renderTarget
*/

namespace artemis
{

class RenderComponent : public Component
{
public:
    RenderComponent()
        :Component(ctRender), renderTarget(nullptr), renderDrawable(nullptr), visible(true)
    {};

    ~RenderComponent() {};

    sf::RenderTarget* renderTarget;

    sf::Drawable* renderDrawable;

    bool visible;
};

};

#endif // RENDER_COMPONENT_H
