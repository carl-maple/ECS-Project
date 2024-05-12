#pragma once
#include "Utils/Base.h"
#include <vector>
#include "SupportPoint.h"

class CGJKSolver
{
public:
	bool Intersect(const std::vector<CVector2f>& shape_lhs, const std::vector<CVector2f>& shape_rhs, std::vector<SSupportPoint>& support_points);
	bool DoSimplex(std::vector<SSupportPoint>& support_points, CVector2f& direction);
	bool DoSimplexEdge(const uint32 index_a, const uint32 index_b, std::vector<SSupportPoint>& support_points, CVector2f& direction);
private:
};

