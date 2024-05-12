#include "AABox.h"
#include <cstdlib>

CAABox::CAABox()
{
	Min = {};
	Max = {};
}

CAABox::CAABox(const CVector2f& InMin, const CVector2f& InMax)
:Min(InMin)
,Max(InMax)
{
}

CAABox CAABox::CreateAABoxFromPositionSize(const CVector2f& InPosition, const CVector2f& InSize)
{
	const CVector2f Min = InPosition - (InSize * 0.5f);
	const CVector2f Max = InPosition + (InSize * 0.5f);
	return CAABox(Min, Max);
}

CAABox::~CAABox()
{
}

CVector2f CAABox::GetSize() const
{
	return CVector2f(Max.x - Min.x, Max.y - Min.y);
}

CAABox CAABox::GetDifference(const CAABox& InOther) const
{
	CAABox Difference;
	Difference.Min.x = Min.x - InOther.Max.x;
	Difference.Max.y = InOther.Max.y - Min.y;
	Difference.Max.x = Max.x - InOther.Min.x;
	Difference.Min.y = InOther.Min.y - Max.y;

	return Difference;
}

bool CAABox::IsOverlapping(const CAABox& InOther) const
{
	const CAABox Difference = GetDifference(InOther);

	if (Difference.Min.x <= 0 &&
		Difference.Max.x >= 0 &&
		Difference.Min.y <= 0 &&
		Difference.Max.y >= 0)
	{
		return true;
	}

	return false;
}

bool CAABox::IsOverlapping(const CAABox& InOther, CVector2f& PenetrationVector) const
{
	const CAABox Difference = GetDifference(InOther);

	if (Difference.Min.x <= 0 &&
		Difference.Max.x >= 0 &&
		Difference.Min.y <= 0 &&
		Difference.Max.y >= 0)
	{
		PenetrationVector = Difference.ClosestPointOnBoundsToPoint(CVector2f::Zero());
		return true;
	}

	return false;
}

CVector2f CAABox::GetPenetrationVector(const CAABox& InOther)
{
	return GetDifference(InOther).ClosestPointOnBoundsToPoint(CVector2f::Zero());
}

CVector2f CAABox::ClosestPointOnBoundsToPoint(const CVector2f& InPoint) const
{
	CVector2f BoundsPoint = CVector2f(Min.x, InPoint.y);
	float MinDist = std::abs(InPoint.x - Min.x);

	if (std::abs(Max.x - InPoint.x) < MinDist)
	{
		MinDist = std::abs(Max.x - InPoint.x);
		BoundsPoint = CVector2f(Max.x, InPoint.y);
	}
	if (std::abs(Max.y - InPoint.y) < MinDist)
	{
		MinDist = std::abs(Max.y - InPoint.y);
		BoundsPoint = CVector2f(InPoint.x, Max.y);
	}
	if (std::abs(Min.y - InPoint.y) < MinDist)
	{
		MinDist = std::abs(Min.y - InPoint.y);
		BoundsPoint = CVector2f(InPoint.x, Min.y);
	}

	return BoundsPoint;
}

std::vector<CVector2f> CAABox::GetVertices() const
{
	std::vector<CVector2f> Vertices;
	Vertices.reserve(4);

	Vertices.push_back(Min);
	Vertices.push_back(Max);
	Vertices.push_back(CVector2f(Min.x, Max.y));
	Vertices.push_back(CVector2f(Max.x, Min.y));

	return Vertices;
}

CVector2f AABBSupport(const CAABox& InBox, const CVector2f& InVector)
{
	CVector2f Ret;

	Ret.x = InVector.x >= 0.f ? InBox.Max.x : InBox.Min.x;
	Ret.y = InVector.y >= 0.f ? InBox.Max.y : InBox.Min.y;

	return Ret;
}