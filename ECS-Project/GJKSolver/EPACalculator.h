#pragma once

#include "Types/Vector2f.h"
#include "EPAContactInfo.h"
#include "SupportPoint.h"
#include <vector>

struct SSupportPoint;

class CEPACalculator
{
public:
	CEPACalculator();
	~CEPACalculator();

	SEPAContactInfo GetContactInfo(const std::vector<CVector2f>& shape_lhs, const std::vector<CVector2f>& shape_rhs, std::vector<SSupportPoint>& support_points);
	
private:

	struct SEdge
	{
		CVector2f m_A = CVector2f::Zero();
		CVector2f m_B = CVector2f::Zero();
		CVector2f m_Normal = CVector2f::Zero();
		uint32 m_Index = 0;
		float m_Distance = 0.f;
	};

	CEPACalculator::SEdge FindClosesetEdge(const std::vector<SSupportPoint>& support_points);
};

