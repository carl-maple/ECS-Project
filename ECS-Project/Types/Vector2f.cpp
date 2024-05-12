#include "Vector2f.h"
#include "Vector2i.h"

const CVector2f CVector2f::operator+(const CVector2i& in) const
{
	return CVector2f(x + in.x, y + in.y);
}