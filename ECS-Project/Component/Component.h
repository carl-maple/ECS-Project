#pragma once

#include "Utils/Base.h"

struct SInstanceIndex
{
	SInstanceIndex(const uint32& index) :m_Index(index) {}
	operator uint32() const { return m_Index; }
private:
	uint32 m_Index;
};

class IComponent
{
public:
	virtual ~IComponent() {};
protected:
	IComponent() {};
};

