#pragma once

#include "System/System.h"
#include "SupportPoint.h"
#include "GJKSolver/GJKSolver.h"

class CCollisionSystem : public CSystem
{
public:
	CCollisionSystem();
	~CCollisionSystem();
	void Update(const float dt);
private:
	CGJKSolver m_Solver;
	std::vector<SSupportPoint> m_SupportPoints;
};

