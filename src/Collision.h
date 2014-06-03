#ifndef _COLLISION_H_
#define _COLLISION_H_
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <cassert>
#include <vector>
#include <algorithm>
#include <cfloat>
#include <cmath>

//TODO: read GJK tutorial again

/**
* GJK algorithm, for 2d only.
*
* GJK only works for convex shapes and I'm using SFML's sf::ConvexShape.
*/

namespace ye
{

typedef std::vector<sf::Vector2f> Simplex;

/**
* Scalar dot product for 2d vectors.
*/
float dot(const sf::Vector2f& v1, const sf::Vector2f& v2);

/**
* Scalar dot product for 3d vectors.
*/
float dot(sf::Vector3f v1, sf::Vector3f v2);

/**
* Product of three 2d vectors.
*/
sf::Vector2f tripleProduct(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c);

/**
* Find if two rectangles are intersecting.
*/
bool isRectIntersecting(const sf::FloatRect& a, const sf::FloatRect& b);

/**
* Returns the Furthest
*/
size_t FurthestPointInDirection(const sf::ConvexShape& shape, const  sf::Vector2f& direction);

/**
* Support Function, supports GJK *ding*!
*/
sf::Vector2f Support(const sf::ConvexShape& shape1, const sf::ConvexShape& shape2, const sf::Vector2f& direction);

/**
* Does the Simplex contain a point (in this case the Origin).
*/
bool containsOrigin(Simplex& simplex, sf::Vector2f& d);

/**
* GJK algorithm, outputting whether 2 shapes intersecting?
*/
bool isIntersecting(const sf::ConvexShape& A,const sf::ConvexShape& B, Simplex& simplex);

typedef struct
{
    float distance;
    sf::Vector2f normal;
    size_t index; // unsigned int
} Edge;

/**
* Find the closest side of a shape.
*/
Edge findClosestEdge(const Simplex& simplex);


/**
* Gotta know that penetration distance.
* Returns how deep shape A went into B.
*/
sf::Vector2f findPenetrationDistance(const sf::ConvexShape& A, const sf::ConvexShape& B, Simplex& simplex);

};

#endif // _COLLISION_H_




