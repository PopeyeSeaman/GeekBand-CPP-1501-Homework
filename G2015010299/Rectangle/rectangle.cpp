#include <iostream>
#include "Rectangle.h"

using std::cout;
using std::endl;

int main()
{
    Rectangle rect(50, 50, 0, 0);
    cout << "constructed a rectangle." << endl;

    Rectangle rect1(rect);
    cout << "copy constructed a rectangle." << endl;

    Rectangle rect2;
    rect2 = rect;
    cout << "copy assigned a rectangle." << endl;

    return 0;
}

