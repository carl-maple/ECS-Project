#pragma once

#include "Types/Vector2f.h"
#include <vector>

struct SSupportPoint
{
	CVector2f Position;
	CVector2f m_OrginA;
	CVector2f m_OrginB;
	CVector2f m_Direction;

	static SSupportPoint GetSupportPoint(const std::vector<CVector2f>& vertices_lhs, const std::vector<CVector2f>& vertices_rhs, const CVector2f& direction);
};

