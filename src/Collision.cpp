#include "Collision.h"

namespace ye
{

//scalar
float dot(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
    return (v1.x * v2.x + v1.y * v2.y);
}

float dot(sf::Vector3f v1, sf::Vector3f v2)
{
    return (v1.x * v2.x + v1.y * v2.y + v1.z *v2.z);
}

sf::Vector2f tripleProduct(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c)
{
    //for the purpose of 2d I'm guessing there is no cross product in this
    //for vectors a * b * c =  b * (a * c) - c * (a * b)

    //a * c
    float axc = dot(a, c);

    //a * b
    float axb = dot(a, b);

    //b * (a * c)
    sf::Vector2f bxaxc = b * axc;

    //c * (a * b)
    sf::Vector2f cxaxb = c * axb;

    //b * (a * c) - c * (a * b)
    return bxaxc - cxaxb;
}

bool isRectIntersecting(const sf::FloatRect& a, const sf::FloatRect& b)
{
    if (a.top + a.height < b.top) return false;
    if (a.top > b.top + b.height) return false;

    if (a.left + a.width < b.left) return false;
    if (a.left > b.left + b.width) return false;

    return true;
}

size_t FurthestPointInDirection(const sf::ConvexShape& shape, const  sf::Vector2f& direction)
{
    // transform is needed to get correct position from rectangleshapes, otherwise you just end up with points at the origin
    // if the algorithm is modified to use another form of shapes which, this can probably be removed
    const sf::Transform& shapeTrans = shape.getTransform();

    float furthestDot = dot(shapeTrans.transformPoint(shape.getPoint(0)), direction);
    size_t furthestIndex = 0;

    for(size_t i = 0; i < shape.getPointCount(); ++i)
    {
        // if the dot product between the point and the diretion is larger than any other point in the shape, it's furthest away
        float curDot = dot(shapeTrans.transformPoint(shape.getPoint(i)), direction);
        if(curDot > furthestDot)
        {
            furthestDot = curDot;
            furthestIndex = i;
        }
    }

    return furthestIndex;
}

sf::Vector2f Support(const sf::ConvexShape& shape1, const sf::ConvexShape& shape2, const sf::Vector2f& direction)
{
    //transforms furthest point in a shape
    sf::Vector2f p1 = shape1.getTransform().transformPoint(shape1.getPoint(FurthestPointInDirection(shape1, direction)));
    sf::Vector2f p2 = shape2.getTransform().transformPoint(shape2.getPoint(FurthestPointInDirection(shape2, -direction)));

    sf::Vector2f p3 = p1 - p2;
    //printf("%f %f\n", p3.x, p3.y);
    return p3;
}



bool containsOrigin(Simplex& simplex, sf::Vector2f& d)
{
    //a, last point in simplex
    sf::Vector2f a = simplex.back();

    //a to o ,origin, is -a
    sf::Vector2f ao = -a;

    if(simplex.size() == 3)
    {
        //it's the triangle case meaning we should check to see if it encompasses the origin
        //get b and c
        sf::Vector2f b = simplex.at(0);
        sf::Vector2f c = simplex.at(1);

        //compute edges
        sf::Vector2f ab = b - a;
        sf::Vector2f ac = c - a;

        // compute the normals
        //in 2D the perpendicular line is, inverse reciprocal, (-y,x)

        sf::Vector2f abPerp = sf::Vector2f(-ab.y, ab.x);

        if(dot(abPerp, c) >= 0)
        {
            // new direction might be "wrong way", check against other side
            abPerp = -abPerp;
        }

        //sf::Vector2f abPerp = tripleProduct(ac, ab, ab);
        //sf::Vector2f acPerp = tripleProduct(ab, ac, ac);

        // is the origin in R4
        if (dot(abPerp, ao) > 0)
        {
            // remove point c
            simplex.erase(std::find(simplex.begin(), simplex.end(), c));
            // set the new direction to abPerp
            d = abPerp;
        }
        else
        {

            //in 2D the perpendicular line is -y,x
            sf::Vector2f acPerp = sf::Vector2f(-ac.y, ac.x);

            if(dot(acPerp, b) >= 0)
            {
                // new direction might be "wrong way", check against other side
                acPerp = -acPerp;
            }


            // is the origin in R3
            if (dot(acPerp, ao) >  0)
            {
                // remove point b
                simplex.erase(std::find(simplex.begin(), simplex.end(), b));

                // set the new direction to acPerp
                d = acPerp;
            }
            else
                return true;

        }
    }
    else if(simplex.size() == 2)
    {
        // then its the line segment case
        sf::Vector2f b = simplex[0];
        // compute AB
        sf::Vector2f ab = b - a;


        // get the perp to AB in the direction of the origin
        sf::Vector2f abPerp = sf::Vector2f(-ab.y, ab.x);

        if(dot(abPerp,ao) < 0)
        {
            abPerp = -abPerp;
        }


        //sf::Vector2f abPerp = tripleProduct(ab, ao, ab);

        // set the direction to abPerp
        d = abPerp;
    }

    //no collision
    return false;
}


bool isIntersecting(const sf::ConvexShape& A,const sf::ConvexShape& B, Simplex& simplex)
{

    sf::Vector2f d(1.f, 1.0f);
    simplex.push_back(Support(A,B,d));

    d = -d;

    int i = 0;
    while(++i < 100)
    {
        // find a new vector in the search direction
        sf::Vector2f v = Support(A,B,d);

        if(dot(v, d) < 0)
            // if we couldn't find a new point in the direction, we stop
            return false;
        else
        {
            //found a point
            simplex.push_back(v);

            bool containOrigin = containsOrigin(simplex, d);

            if (containOrigin)
                return true;

        }
    }

    return false;

}

Edge findClosestEdge(const Simplex& simplex)
{
    Edge closest;
    // prime the distance of the edge to the max
    closest.distance = FLT_MAX;

    // s is the passed in simplex
    for (unsigned int i = 0; i < simplex.size(); i++)
    {
        // compute the next points index
        size_t j = (i + 1 == simplex.size() ? 0 : i + 1);

        // get the current point and the next one
        sf::Vector2f a = simplex[i];
        sf::Vector2f b = simplex[j];

        // create the edge vector
        sf::Vector2f e = b - a; // or a to b;

        // get the vector from the edge towards the origin
        sf::Vector2f n(-e.y, e.x);

        if(dot(n, -a) >= 0)
        {
            n = -n;
        }

        // normalize the vector
        n /= (float)sqrt(dot(n,n)); // normalize!

        // calculate the distance from the origin to the edge
        float d = dot(n, a); // could use b or a here

        // check the distance against the other distances
        if (d < closest.distance)
        {
            // if this edge is closer then use it
            closest.distance = d;
            closest.normal = n;
            closest.index = j;
        }
    }
    // return the closest edge we found
    return closest;
}


sf::Vector2f findPenetrationDistance(const sf::ConvexShape& A, const sf::ConvexShape& B, Simplex& simplex)
{
    int i = 0;
    while(++i < 100) //should not be more than like 6 iterations
    {
        // obtain the feature (edge for 2D) closest to the origin on the Minkowski Difference
        Edge e = findClosestEdge(simplex);
        // obtain a new support point in the direction of the edge normal
        sf::Vector2f p = Support(A, B, e.normal);

        // check the distance from the origin to the edge against the
        // distance p is along e.normal
        float d = dot(p, e.normal);
        if (fabs(d - e.distance) < 0.001f) // the tolerance should be something positive close to zero (ex. 0.00001)
        {
            // if the difference is less than the tolerance then we can
            // assume that we cannot expand the simplex any further and
            // we have our solution
            return ((d+ 0.001f) *e.normal);
        }
        else
        {
            // we haven't reached the edge of the Minkowski Difference
            // so continue expanding by adding the new point to the simplex
            // in between the points that made the closest edge
            simplex.insert(simplex.begin() + e.index, p);
        }
    }
    //return sf::Vector2f();
}

};



