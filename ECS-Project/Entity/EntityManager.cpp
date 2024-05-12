#include "Entity/EntityManager.h"
#include "Component/ComponentTypeManager.h"
#include "World/World.h"
#include "Entity/Entity.h"
#include "System/SystemManager.h"

CEntityManager::CEntityManager()
{
}

CEntityManager::~CEntityManager()
{
}

const SEntityId CEntityManager::CreateEntity()
{
	uint32 Entity_id;

	if (m_FreeIndices.size() > MINIMUM_FREE_INDICES)
	{
		Entity_id = m_FreeIndices.front();
		m_FreeIndices.pop_front();
	}
	else
	{
		m_Generations.push_back(0);
		Entity_id = static_cast<uint32>(m_Generations.size() - 1);
	}

	const SEntityId generated_Entity_id = { Entity_id, m_Generations[Entity_id] };
	m_Entities.emplace(generated_Entity_id);
	return generated_Entity_id;
}

void CEntityManager::DestoryEntity(const SEntityId& Entity_id)
{
	const uint32 index = Entity_id.GetIndex();
	++m_Generations[index];
	m_FreeIndices.push_back(index);
}

bool CEntityManager::IsEntityAlive(const SEntityId& Entity) const
{
	return m_Generations[Entity.GetIndex()] == Entity.GetGeneration();
}

void CEntityManager::AddComponent(const SEntityId& Entity_id, IComponent* component)
{
	const CComponentType& component_type = SystemManager->GetWorld()->GetComponentTypeManager()->GetType(typeid(*component));
	m_ComponentsByType[component_type.GetId()].push_back(component);

	std::map<uint32, IComponent*>& Entity_components = m_ComponentsByEntity[Entity_id.GetId()];
	const std::map<uint32, IComponent*>::const_iterator it = Entity_components.find(component_type.GetId());

	if (it != std::end(Entity_components))
	{
		SDL_Log("CEntityManager::AddComponent - Component already exists");
	}
	else
	{
		Entity_components[component_type.GetId()] = component;
		m_ComponentBitsByEntity[Entity_id] |= (component_type.GetBit());
	}
}

void CEntityManager::AddSystemBit(const SEntityId& Entity_id, const std::bitset<NBitSize::BIT_SIZE> bit)
{
	m_SystemBitsByEntity[Entity_id] |= bit;
	m_EntitiesBySystemBits[bit.to_string()].push_back(Entity_id);
}

const std::bitset<NBitSize::BIT_SIZE>& CEntityManager::GetEntitySystemBits(const SEntityId& Entity_id) const
{
	return m_SystemBitsByEntity[Entity_id];
}

std::vector<SEntityId>& CEntityManager::GetEntities(const std::bitset<NBitSize::BIT_SIZE>& SystemBit)
{
	return m_EntitiesBySystemBits[SystemBit.to_string()];
}

const std::vector<SEntityId>& CEntityManager::GetEntitiesToBeUpdated(const std::bitset<NBitSize::BIT_SIZE>& SystemBit)
{
	std::vector<SEntityId>& entities = GetEntities(SystemBit);
	std::vector<SEntityId>::iterator it = entities.begin();
	const std::vector<SEntityId>::iterator end_it = entities.end();

	for (; it != end_it;)
	{
		if (IsEntityAlive(*it) == false)
		{
			entities.erase(it);
		}
		else
		{
			++it;
		}
	}

	return entities;
}