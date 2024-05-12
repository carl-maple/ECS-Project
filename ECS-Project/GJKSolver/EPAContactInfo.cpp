#include "EPAContactInfo.h"


void SEPAContactInfo::Reset()
{
	m_PositionA = CVector2f::Zero();
	m_PositionB = CVector2f::Zero();
	m_Normal = CVector2f::Zero();
	m_Depth = 0;
}
