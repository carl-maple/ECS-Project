#include "GJKSolver.h"

bool CGJKSolver::Intersect(const std::vector<CVector2f>& ShapeLhs, const std::vector<CVector2f>& ShapeRhs
	, std::vector<SSupportPoint>& OutSupportPoints)
{
	OutSupportPoints.clear();

	CVector2f Direction = ShapeLhs[0]; // Direction doesn't matter so we just pick the first point
	SSupportPoint NextSupportPoint = SSupportPoint::GetSupportPoint(ShapeLhs, ShapeRhs, Direction);
	OutSupportPoints.push_back(NextSupportPoint);

	// Use the negative direction for the next point
	Direction = Direction * -1.f;

	bool Intersection = false;

	while (true)
	{
		NextSupportPoint = SSupportPoint::GetSupportPoint(ShapeLhs, ShapeRhs, Direction);

		// Make sure the point passed the origin
		if (NextSupportPoint.Position.Dot(Direction) <= 0)
		{
			break;
		}

		OutSupportPoints.push_back(NextSupportPoint);

		if (DoSimplex(OutSupportPoints, Direction))
		{
			Intersection = true;
			break;
		}
	}

	return Intersection;
}

bool CGJKSolver::DoSimplex(std::vector<SSupportPoint>& SupportPoints, CVector2f& Direction)
{
	CVector3f NewDirection;
	if (SupportPoints.size() == 1)
	{
		Direction = -SupportPoints[0].Position;
	}
	else if (SupportPoints.size() == 2)
	{
		DoSimplexEdge(1, 0, SupportPoints, Direction);
	}
	else
	{
		const CVector3f a = CVector3f(SupportPoints[2].Position.x, SupportPoints[2].Position.y, 0);
		const CVector3f b = CVector3f(SupportPoints[1].Position.x, SupportPoints[1].Position.y, 0);
		const CVector3f c = CVector3f(SupportPoints[0].Position.x, SupportPoints[0].Position.y, 0);

		const CVector3f ac = c - a;
		const CVector3f ab = b - a;

		const CVector3f abc = CVector3f::Cross3D(ab, ac);

		if (CVector3f::Cross3D(abc, ac).SameDirection(-a))
		{
			if (ac.SameDirection(-a))
			{
				NewDirection = CVector3f::Cross3D(CVector3f::Cross3D(ac, -a), ac);
				Direction.x = NewDirection.x;
				Direction.y = NewDirection.y;
				std::swap(SupportPoints[1], SupportPoints[2]);
				SupportPoints.pop_back();
			}
			else
			{
				DoSimplexEdge(2, 1, SupportPoints, Direction);
			}
		}
		else
		{
			if (CVector3f::Cross3D(ab, abc).SameDirection(-a))
			{
				DoSimplexEdge(2, 1, SupportPoints, Direction);
			}
			else
			{
				return true;
			}
		}
		
	}

	return false;
}

bool CGJKSolver::DoSimplexEdge(const uint32 IndexLhs, const uint32 IndexRhs
	, std::vector<SSupportPoint>& SupportPoints, CVector2f& OutDirection)
{
	CVector3f NewDirection;
	const SSupportPoint& SupportPointLhs = SupportPoints[IndexLhs];
	const SSupportPoint& SupportPointRhs = SupportPoints[IndexRhs];

	CVector2f LhsToRhsVector = SupportPointRhs.Position - SupportPointLhs.Position;
	std::vector<SSupportPoint> NewSupportPoints;
	if (LhsToRhsVector.SameDirection(-SupportPointLhs.Position))
	{
		OutDirection = CVector2f::TripleProduct(LhsToRhsVector, -SupportPointLhs.Position, LhsToRhsVector);

		if (OutDirection.LengthSq() <= std::numeric_limits<float>::epsilon())
		{
			OutDirection = LhsToRhsVector.Left();
		}

		NewSupportPoints.push_back(SupportPointRhs);
		NewSupportPoints.push_back(SupportPointLhs);
		SupportPoints = NewSupportPoints;

		return true;
	}
	else
	{
		OutDirection = -SupportPointLhs.Position;
		NewSupportPoints.push_back(SupportPointLhs);
		SupportPoints = NewSupportPoints;

		return false;
	}
}