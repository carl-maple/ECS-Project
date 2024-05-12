#include "Entity/Entity.h"

CEntity::CEntity(const SEntityId& identifier)
:m_Identifier(identifier)
{

}

CEntity::~CEntity()
{
}

void CEntity::AddSystemBit(const std::bitset<NBitSize::BIT_SIZE> bit)
{
	m_SystemBits |= bit;
}

void CEntity::AddTypeBit(const std::bitset<NBitSize::BIT_SIZE> bit)
{
	m_TypeBits |= bit;
}

const uint32 CEntity::GetId() const 
{
	return m_Identifier.GetId();
}

const uint32 CEntity::GetGeneration() const
{
	return m_Identifier.GetGeneration();
}

const SEntityId CEntity::GetIdentifier() const 
{
	return m_Identifier;
}

const std::bitset<NBitSize::BIT_SIZE>& CEntity::GetSystemBits() const
{
	return m_SystemBits;
}

const std::bitset<NBitSize::BIT_SIZE>& CEntity::GetTypeBits() const
{
	return m_TypeBits;
}