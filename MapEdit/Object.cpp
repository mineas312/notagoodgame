#include "Object.h"

void Object::set(int width, int height, int x, int y, bool _canMoveThrough, int _id)
{
	box.w = width;
	box.h = height;
	box.x = x;
	box.y = y;
	canMoveThrough = _canMoveThrough;
	id = _id;
}
