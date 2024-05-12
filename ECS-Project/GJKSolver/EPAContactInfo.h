#pragma once

#include "Types/Vector2f.h"
#include "Types/Types.h"

struct SEPAContactInfo
{
	void Reset();

	CVector2f m_PositionA = CVector2f::Zero();
	CVector2f m_PositionB = CVector2f::Zero();
	CVector2f m_Normal = CVector2f::Zero();
	int32 m_Depth = 0;
};

