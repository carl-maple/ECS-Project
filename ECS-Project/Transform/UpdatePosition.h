#pragma once
#include "System/System.h"

class CUpdatePosition : public CSystem
{
public:
	CUpdatePosition();
	~CUpdatePosition();
	void Update(const float dt);
private:
};

