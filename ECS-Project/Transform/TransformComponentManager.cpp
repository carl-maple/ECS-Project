#include "Transform/TransformComponentManager.h"
#include <Windows.h>


CTransformComponentManager::CTransformComponentManager()
{
	Allocate(NUM_TRANSFORM_COMPONENTS);
}

CTransformComponentManager::~CTransformComponentManager()
{
	VirtualFree(m_TransformData.Buffer, m_TransformData.m_NumAllocated, 0);
}

void CTransformComponentManager::Allocate(const uint32 size)
{
	SInstanceData new_data = {};
	const uint32 bytes = size * (sizeof(SEntityId) + sizeof(CVector2f));
	new_data.Buffer = VirtualAlloc(0, bytes, MEM_COMMIT, PAGE_READWRITE);
	new_data.m_NumUsed = m_TransformData.m_NumUsed;
	new_data.m_NumAllocated = size;

	new_data.m_Entities = static_cast<SEntityId*>(new_data.Buffer);
	new_data.m_Position = reinterpret_cast<CVector2f*>(new_data.m_Entities + size);

	memcpy(new_data.m_Entities, m_TransformData.m_Entities, m_TransformData.m_NumUsed * sizeof(SEntityId));
	memcpy(new_data.m_Position, m_TransformData.m_Position, m_TransformData.m_NumUsed * sizeof(CVector2f));

	VirtualFree(m_TransformData.Buffer, m_TransformData.m_NumAllocated, 0);

	m_TransformData = new_data;
}

const SInstanceIndex CTransformComponentManager::GetInstance(const SEntityId& Entity) const
{
	return m_EntityMap[Entity];
}

const CVector2f& CTransformComponentManager::GetPosition(const SInstanceIndex& Instance) const
{
	return m_TransformData.m_Position[Instance];
}

const CVector2f& CTransformComponentManager::GetPosition(const SEntityId& Entity) const
{
	const SInstanceIndex InstanceIndex = GetInstance(Entity);
	return m_TransformData.m_Position[InstanceIndex];
}

CVector2f& CTransformComponentManager::GetPosition(const SEntityId& Entity)
{
	return const_cast<CVector2f&>(const_cast<const CTransformComponentManager*>(this)->GetPosition(Entity));
}

void CTransformComponentManager::SetPosition(const SEntityId& Entity, const CVector2f& position)
{
	const SInstanceIndex InstanceIndex = GetInstance(Entity);
	return m_TransformData.m_Position[InstanceIndex] = position;
}

void CTransformComponentManager::DestroyInstance(const SInstanceIndex& Instance)
{
	const size_t last = m_TransformData.m_NumUsed - 1;
	const SEntityId Entity = m_TransformData.m_Entities[Instance];
	const SEntityId LastEntity = m_TransformData.m_Entities[last];

	m_TransformData.m_Entities[Instance] = m_TransformData.m_Entities[last];
	m_TransformData.m_Position[Instance] = m_TransformData.m_Position[last];

	m_EntityMap[LastEntity] = Instance;
	m_EntityMap.erase(Entity);

	--m_TransformData.m_NumUsed;
}

const SInstanceIndex CTransformComponentManager::CreateInstance(const SEntityId& Entity, const CVector2f& position)
{
	const uint32 InstanceIndex = m_TransformData.m_NumUsed;
	++m_TransformData.m_NumUsed;

	m_TransformData.m_Entities[InstanceIndex] = Entity;
	m_TransformData.m_Position[InstanceIndex] = position;

	m_EntityMap.insert(std::make_pair(Entity, InstanceIndex));

	return SInstanceIndex(InstanceIndex);
}

const CVector2f* CTransformComponentManager::GetInstances(uint32& NumInstances) const
{
	NumInstances = m_TransformData.m_NumUsed;
	return m_TransformData.m_Position;
}