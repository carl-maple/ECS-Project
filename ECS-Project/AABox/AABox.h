#pragma once

#include "Types/Vector2f.h"
#include <vector>

class CAABox
{
public:

	CAABox();
	CAABox(const CVector2f& InMin, const CVector2f& InMax);
	static CAABox CreateAABoxFromPositionSize(const CVector2f& position, const CVector2f& size);
	~CAABox();
	CVector2f GetSize() const;
	
	CAABox GetDifference(const CAABox& InOther) const;
	bool IsOverlapping(const CAABox& InOther) const;
	bool IsOverlapping(const CAABox& InOther, CVector2f& PenetrationVector) const;
	CVector2f GetPenetrationVector(const CAABox& InOther);
	CVector2f ClosestPointOnBoundsToPoint(const CVector2f& point) const;

	std::vector<CVector2f> GetVertices() const;

	CVector2f Min;
	CVector2f Max;
	
};

