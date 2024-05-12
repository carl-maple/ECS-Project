#pragma once
#include <bitset>
#include "Component/BitSize.h"
#include "Utils/Base.h"

class CComponentType
{
public:
	CComponentType();
	~CComponentType();

	const std::bitset<NBitSize::BIT_SIZE>& GetBit() const;
	const uint32& GetId() const;
	void Reset();

private:
	static std::bitset<NBitSize::BIT_SIZE> s_NextBit;
	static uint32 s_NextId;

	std::bitset<NBitSize::BIT_SIZE> m_Bit;
	uint32 m_Id;

};

