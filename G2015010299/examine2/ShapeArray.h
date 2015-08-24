#include <ctime>
#include <cstdlib>
#include "Rectangle.h"
#include "Circle.h"

#define ARY_SHAPES_CAPACITY 20				// 图形数组容量
#define RAND_MAX_LIMIT 10				// 随机数大小的上限
#define EXPRESS_RANDNUM rand() % RAND_MAX_LIMIT + 1 	// 求1～10随机数的表达式
#define DEL_SHAPE_LIMIT 50				// 作为删除对象的图形的面积界定值（小于则删除）

class ShapeArray
{
    Shape** ary_shapes;		// 图形指针数组
    short _capacity;		// 图形指针数组容量
    short _size;		// 图形指针数组内的现有元素个数

public:
    ShapeArray(short capacity) : _capacity(capacity), _size(0)
    {
	ary_shapes = new Shape*[_capacity];
    }

    void fill_rectangles(unsigned short quantity);
    void fill_circles(unsigned short quantity);
    void del_small_shapes(int area_limit);
    void realloc_min_capacity();
    Shape** getShapeAry() const { return ary_shapes; }
    short getCapacity() const { return _capacity; }
    short getSize() const { return _size; }

    ~ShapeArray()
    {
	for (short i = 0; i < _size; i++)
	{
	    delete ary_shapes[i];
	}
	delete[] ary_shapes;
    }

private:
    void regular_shapes();

};

/* 填充Rectangle到图形指针数组中。参数quantiry是填充个数 */
void ShapeArray::fill_rectangles(unsigned short quantity)
{
    if (_size + quantity > ARY_SHAPES_CAPACITY)
	quantity = ARY_SHAPES_CAPACITY - _size; 

    srand(int(time(0)));
    for (short i = _size; i < _size + quantity; i++)
    {
	int width = EXPRESS_RANDNUM;
	int height = EXPRESS_RANDNUM;
	int x = EXPRESS_RANDNUM;
	int y = EXPRESS_RANDNUM;

	ary_shapes[i] = new Rectangle(i+1, width, height, x, y);
    }
    _size += quantity;
}

/* 填充Circle到图形指针数组中。参数quantiry是填充个数 */
void ShapeArray::fill_circles(unsigned short quantity)
{
    if (_size + quantity > ARY_SHAPES_CAPACITY)
	quantity = ARY_SHAPES_CAPACITY - _size; 

    srand(int(time(0)));
    for (short i = _size; i <  _size + quantity; i++)
    {
	int x = EXPRESS_RANDNUM;
	int y = EXPRESS_RANDNUM;
	int radius = EXPRESS_RANDNUM;

	ary_shapes[i] = new Circle(i+1, x, y, radius);
    }
    _size += quantity;
}

/* 删除图形指针数组中的小面积图形，
   并将剩余的图形指针按顺序移到数组前部。
   参数area_limit指定删除对象的面积界定值。*/
void ShapeArray::del_small_shapes(int area_limit)
{
    for (short i = 0; i < _size; i++)
    {
	if (ary_shapes[i]->getArea() < DEL_SHAPE_LIMIT)
	{
	    delete ary_shapes[i];
	    ary_shapes[i] = nullptr;
	}
    }
    regular_shapes();
}

/* 将现存的图形指针按顺序移到图形指针数组前部。
   private函数，只在del_small_shapes(int)中被调用。*/
void ShapeArray::regular_shapes()
{
    short last = 0;  // 数组中第一个空（nullptr）元素的索引
    for (short i = 0; i < _size; i++)
    {
	if (ary_shapes[i] != nullptr)
	{
	    // 将数组中的非空元素移到第一个空元素的位置，
	    // 如果在它的位置之前没有空元素则不作处理。
	    if (i > last)
	    {
		ary_shapes[last] = ary_shapes[i];
		ary_shapes[i] = nullptr;
	    }
	    last++;
	}
    }

    // 相应调整数组size
    if (last != 0)
	_size = last;
}

/* 把图形指针数组的容量调整到与现有元素个数一致。*/
void ShapeArray::realloc_min_capacity()
{
    ary_shapes = (Shape**)realloc(ary_shapes, sizeof(Shape*) * _size);
    _capacity = _size;
}

