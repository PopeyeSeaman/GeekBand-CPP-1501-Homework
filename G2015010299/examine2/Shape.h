#ifndef __SHAPE__
#define __SHAPE__

class Shape
{
    int no;

public:
    Shape(int no) : no(no) {}
    int getNo() const { return no; }
    
    virtual int getArea() const = 0;
    virtual ~Shape() {}
};

#endif

