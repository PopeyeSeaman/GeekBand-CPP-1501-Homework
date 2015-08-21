#ifndef __RECTANGLE__
#define __RECTANGLE__

#include "Shape.h"
#include "Point.h"

class Rectangle: public Shape
{
    int width;
    int height;

    Point leftUp;

public:
    Rectangle(int no, int width, int height, int x, int y): 
        Shape(no), width(width), height(height), leftUp(Point(x, y))
    {}
    
    virtual int getArea() const
    {
	return width * height;
    }
};

#endif

