#pragma once

#include "System/System.h"

class CPlayerInputMovementSystem : public CSystem
{
public:
	CPlayerInputMovementSystem();
	~CPlayerInputMovementSystem();
	void Update(const float dt) override;
};

