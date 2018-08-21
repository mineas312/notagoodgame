#include "stdafx.h"

#include "Entity.h"

void Entity::del() noexcept
{
	delete[] name;
	name = NULL;
}

void Entity::setEntity(char * _name, const int xPos, const int yPos) noexcept
{
	name = _name;
	box.x = xPos;
	box.y = yPos;
}

void Entity::moveInterpolated(int _x, int _y)
{
	interTicks = 100;
	float tmpX = (_x - box.x) / 100.0f;
	float tmpY = (_y - box.y) / 100.0f;

	interRemainX = std::modf(tmpX, &tmpX);
	interRemainY = std::modf(tmpY, &tmpY);

	interX = (int)tmpX;
	interY = (int)tmpY;
}

void Entity::tickInterpolated()
{
	if (interTicks > 0)
	{
		box.x += interX;
		box.y += interY;

		sumX += interRemainX;
		sumY += interRemainY;

		if (sumX >= 1.0f)
		{
			box.x += 1;
			sumX -= 1.0f;
		}
		else if (sumX <= -1.0f)
		{
			box.x += -1;
			sumX -= -1.0f;
		}
		if (sumY >= 1.0f)
		{
			box.y += 1;
			sumY -= 1.0f;
		}
		else if (sumY <= -1.0f)
		{
			box.y += -1;
			sumY -= -1.0f;
		}

		interTicks--;

		if (interTicks == 0)
		{
			if (sumX > 0.49f)
				box.x += 1;
			else if (sumX < -0.49f)
				box.x += -1;
			if (sumY > 0.49f)
				box.y += 1;
			else if (sumY < -0.49f)
				box.y += -1;
		}
	}
}
