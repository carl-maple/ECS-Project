#pragma once
#include <bitset>
#include "Component/BitSize.h"
#include "Types/Types.h"

constexpr uint32 Entity_INDEX_BITS = 22;
constexpr uint32 Entity_INDEX_MASK = (1 << Entity_INDEX_BITS) - 1;

constexpr uint32 Entity_GENERATION_BITS = 8;
constexpr uint32 Entity_GENERATION_MASK = (1 << Entity_GENERATION_BITS) - 1;

constexpr uint32 MINIMUM_FREE_INDICES = 5;

constexpr uint32 INVALID_Entity_ID = std::numeric_limits<uint32>::max();

struct SEntityId
{
	SEntityId(const uint32& id, const uint8& generation)
	{
		m_Id = (generation << Entity_INDEX_BITS) | id;
	}

	SEntityId()
	{
		m_Id = INVALID_Entity_ID;
	}

	operator uint32() const { return m_Id; }

	const uint32 GetIndex() const { return m_Id & Entity_INDEX_MASK; }
	const uint32 GetGeneration() const { return (m_Id >> Entity_INDEX_BITS) & Entity_GENERATION_MASK; }

	const uint32& GetId() const { return m_Id; }

private:
	uint32 m_Id;
};


class CEntity
{
public:
	CEntity(const SEntityId& identifer);
	~CEntity();

	void AddSystemBit(const std::bitset<NBitSize::BIT_SIZE> bit);
	void AddTypeBit(const std::bitset<NBitSize::BIT_SIZE> bit);

	const uint32 GetId() const;
	const uint32 GetGeneration() const;
	const SEntityId GetIdentifier() const;

	const std::bitset<NBitSize::BIT_SIZE>& GetSystemBits() const;
	const std::bitset<NBitSize::BIT_SIZE>& GetTypeBits() const;
private:
	SEntityId m_Identifier;

	std::bitset<NBitSize::BIT_SIZE> m_SystemBits;
	std::bitset<NBitSize::BIT_SIZE> m_TypeBits;

	// No copy constructor
	CEntity(const CEntity&);
	// No assign operator
	CEntity& operator=(const CEntity&);
};

