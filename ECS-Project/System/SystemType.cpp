#include "SystemType.h"

uint32 CSystemType::s_NextId = 1;
std::bitset<NBitSize::BIT_SIZE> CSystemType::s_NextBit(1);

CSystemType::CSystemType()
{
	m_Bit = s_NextBit;
	s_NextBit = s_NextBit << 1;
	m_Id = s_NextId++;
}

CSystemType::~CSystemType()
{
}

const std::bitset<NBitSize::BIT_SIZE>& CSystemType::GetBit() const
{
	return m_Bit;
}

const uint32& CSystemType::GetId() const
{
	return m_Id;
}

void CSystemType::Reset()
{
	s_NextId = 1;
	s_NextBit = 1;
}