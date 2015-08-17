#ifndef __RECTANGLE__
#define __RECTANGLE__

#include "Share.h"
#include "Point.h"

class Rectangle: public Shape
{
    int width;
    int height;

    Point* leftUp;

public:
    Rectangle():
        width(0), height(0), leftUp(new Point(0, 0)) {}

    Rectangle(int width, int height, int x, int y): 
        width(width), height(height), leftUp(new Point(x, y)) {}
    
    Rectangle(const Rectangle& other):
        Shape(other), width(other.width), height(other.height)
    {
	if (other.leftUp != nullptr)
	    this->leftUp = new Point(*other.leftUp);
	else
	    this->leftUp = nullptr;
    }
    
    Rectangle& operator=(const Rectangle& other)
    {
	if (this == &other)        
	    return *this;

	Shape::operator=(other);

	this->width = other.width;
        this->height = other.height;

	if (other.leftUp != nullptr)
	{
	    if (this->leftUp != nullptr)
		*this->leftUp = *other.leftUp;
	    else 
		this->leftUp = new Point(*other.leftUp);
	}
	else
	{
	    delete this->leftUp;
	    this->leftUp = nullptr;
	}

        return *this;
    }

    ~Rectangle() { delete leftUp; }
};

#endif

