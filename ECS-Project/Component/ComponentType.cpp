#include "Component/ComponentType.h"

uint32 CComponentType::s_NextId = 1;
std::bitset<NBitSize::BIT_SIZE> CComponentType::s_NextBit(1);

CComponentType::CComponentType()
{
	m_Bit = s_NextBit;
	s_NextBit = s_NextBit << 1;
	m_Id = s_NextId++;
}

CComponentType::~CComponentType()
{
}

const std::bitset<NBitSize::BIT_SIZE>& CComponentType::GetBit() const
{
	return m_Bit;
}

const uint32& CComponentType::GetId() const 
{
	return m_Id;
}

void CComponentType::Reset()
{
	s_NextId = 1;
	s_NextBit = 1;
}