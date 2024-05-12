#pragma once

#include "System/System.h"

#include <set>
#include <deque>
#include <vector>
#include <map>
#include <typeindex>

#include "Utils/Base.h"
#include "Component/Component.h"
#include "Entity/Entity.h"

class CEntity;

class CEntityManager : public CSystem
{
public:
	CEntityManager();
	~CEntityManager();
	const SEntityId CreateEntity();
	void DestoryEntity(const SEntityId& Entity);
	bool IsEntityAlive(const SEntityId& Entity) const;
	void AddComponent(const SEntityId& Entity, IComponent* component);
	void AddSystemBit(const SEntityId& Entity_id, const std::bitset<NBitSize::BIT_SIZE> bit);
	const std::bitset<NBitSize::BIT_SIZE>& GetEntitySystemBits(const SEntityId& Entity_id) const;
	const std::vector<SEntityId>& GetEntitiesToBeUpdated(const std::bitset<NBitSize::BIT_SIZE>& SystemBit);
	const std::set<SEntityId>& GetEntities() const { return m_Entities; }
private:
	std::vector<SEntityId>& GetEntities(const std::bitset<NBitSize::BIT_SIZE>& SystemBit);


	std::set<SEntityId> m_Entities;
	std::vector<uint8> m_Generations;
	std::deque<uint32> m_FreeIndices;

	std::map<uint32, std::vector<IComponent*>> m_ComponentsByType;
	std::map<uint32, std::map<uint32, IComponent*>> m_ComponentsByEntity;
	std::map<uint32, std::bitset<NBitSize::BIT_SIZE>> m_ComponentBitsByEntity;
	mutable std::map<uint32, std::bitset<NBitSize::BIT_SIZE>> m_SystemBitsByEntity;
	mutable std::map<std::string, std::vector<SEntityId>> m_EntitiesBySystemBits;

	
};

