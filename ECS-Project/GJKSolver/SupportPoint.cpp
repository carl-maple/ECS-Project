#include "SupportPoint.h"

SSupportPoint SSupportPoint::GetSupportPoint(const std::vector<CVector2f>& VerticesLhs
	, const std::vector<CVector2f>& VerticesRhs, const CVector2f& Direction)
{
	SSupportPoint Result;
	Result.m_Direction = Direction;

	CVector2f CurrentDirection = Direction;

	float Max = -std::numeric_limits<float>::max();
	float Dot;
	
	// Find furthest point in direction
	for (const CVector2f& Vertex : VerticesLhs)
	{
		Dot = CurrentDirection.Dot(Vertex);
		if (Dot > Max)
		{
			Max = Dot;
			Result.m_OrginA = Vertex;
		}
	}

	// Find furthest point but in the opposite direction
	CurrentDirection = -CurrentDirection;
	Max = -std::numeric_limits<float>::max();
	for (const CVector2f& vertex : VerticesRhs)
	{
		Dot = CurrentDirection.Dot(vertex);
		if (Dot > Max)
		{
			Max = Dot;
			Result.m_OrginB = vertex;
		}
	}

	// Minkowski Difference
	Result.Position = Result.m_OrginA - Result.m_OrginB;

	return Result;
}
