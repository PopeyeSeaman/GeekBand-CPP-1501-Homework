#ifndef __CIRCLE__
#define __CIRCLE__

#include "Shape.h"
#include "Point.h"

class Circle: public Shape
{
    Point center;
    int radius;

public:
    Circle(int no, int x, int y, int r): 
        Shape(no), center(Point(x, y)), radius(r)
    {}
    
    virtual int getArea() const
    {
	return int(3.14 * radius * radius);
    }
};

#endif

