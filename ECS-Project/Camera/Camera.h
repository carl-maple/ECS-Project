#pragma once

#include "Types/Vector2f.h"

class CCamera
{
public:
	CCamera();
	~CCamera();
	const CVector2f& GetPosition() const { return m_Position; }
	void SetPosition(const CVector2f& pos) { m_Position = pos; }

private:
	CVector2f m_Position;
};

