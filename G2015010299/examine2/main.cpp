#include <iostream>
#include "ShapeArray.h"

using std::cout;
using std::endl;

int main()
{
    ShapeArray shapeAry(ARY_SHAPES_CAPACITY); 
    cout << "constructed ShapeArray." << endl;

    shapeAry.fill_rectangles(10);
    cout << "filled 10 Rectangle." << endl;

    shapeAry.fill_circles(10);
    cout << "filled 10 Circle." << endl;

    shapeAry.del_small_shapes(DEL_SHAPE_LIMIT);
    cout << "deleted small shapes." << endl;

    shapeAry.realloc_min_capacity();
    cout << "realloc min capacity." << endl;

    cout << "capacity=" << shapeAry.getCapacity() 
	 << ", size=" << shapeAry.getSize() << endl;

    cout << "Shapes no list are: ";
    Shape** ary_shapes = shapeAry.getShapeAry();
    short size = shapeAry.getSize(); 
    for (short i = 0; i < size; i++)
	cout << ary_shapes[i]->getNo() << " ";
    cout << endl;

    return 0;
}

