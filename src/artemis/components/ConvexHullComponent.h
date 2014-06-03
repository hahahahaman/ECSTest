#ifndef CONVEX_HULL_COMPONENT_H

#include <artemis/Component.h>

#include <LTBL/Light/ConvexHull.h>

#include "ComponentTypes.h"

//class ltbl::LightSystem;

namespace artemis
{

class ConvexHullComponent : public Component
{
public:
    ConvexHullComponent()
        :Component(ctConvexHull), lightSystem(nullptr), convexHull(nullptr), isVisible(true), inLightSystem(false)
    {}

    ltbl::LightSystem* lightSystem;

    ltbl::ConvexHull* convexHull;

    bool isVisible;
    bool inLightSystem;

};

};


#endif // CONVEX_HULL_COMPONENT_H
