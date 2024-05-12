#include "Graphics/TextureComponentManager.h"
#include <windows.h>


CTextureComponentManager::CTextureComponentManager()
{
	Allocate(NUM_TEXTURE_COMPONENTS);
}


CTextureComponentManager::~CTextureComponentManager()
{
	VirtualFree(m_TextureData.Buffer, m_TextureData.m_NumAllocated, 0);
}

void CTextureComponentManager::Allocate(const uint32 size)
{
	SInstanceData new_data = {};
	const uint32 bytes = size * (sizeof(SEntityId) + sizeof(size_t) + sizeof(CVector2f));
	new_data.Buffer = VirtualAlloc(0, bytes, MEM_COMMIT, PAGE_READWRITE);
	new_data.m_NumUsed = m_TextureData.m_NumUsed;
	new_data.m_NumAllocated = size;

	new_data.m_Entities = static_cast<SEntityId*>(new_data.Buffer);
	new_data.m_TextureHash = reinterpret_cast<size_t*>(new_data.m_Entities + size);
	new_data.m_Size = reinterpret_cast<CVector2f*>(new_data.m_TextureHash + size);

	memcpy(new_data.m_Entities, m_TextureData.m_Entities, m_TextureData.m_NumUsed * sizeof(SEntityId));
	memcpy(new_data.m_TextureHash, m_TextureData.m_TextureHash, m_TextureData.m_NumUsed * sizeof(size_t));
	memcpy(new_data.m_Size, m_TextureData.m_Size, m_TextureData.m_NumUsed * sizeof(CVector2f));

	VirtualFree(m_TextureData.Buffer, m_TextureData.m_NumAllocated, 0);

	m_TextureData = new_data;
}

const SInstanceIndex CTextureComponentManager::GetInstance(const SEntityId& Entity) const
{
	return m_EntityMap[Entity];
}

void CTextureComponentManager::DestroyInstance(const SInstanceIndex& Instance)
{
	const size_t last = m_TextureData.m_NumUsed - 1;
	const SEntityId Entity = m_TextureData.m_Entities[Instance];
	const SEntityId LastEntity = m_TextureData.m_Entities[last];

	m_TextureData.m_Entities[Instance] = m_TextureData.m_Entities[last];
	m_TextureData.m_TextureHash[Instance] = m_TextureData.m_TextureHash[last];
	m_TextureData.m_Size[Instance] = m_TextureData.m_Size[last];
	
	m_EntityMap[LastEntity] = Instance;
	m_EntityMap.erase(Entity);

	--m_TextureData.m_NumUsed;
}

const SInstanceIndex CTextureComponentManager::CreateInstance(const SEntityId& Entity, const size_t texture_hash, const CVector2f& size)
{
	const uint32 InstanceIndex = m_TextureData.m_NumUsed;
	++m_TextureData.m_NumUsed;

	m_TextureData.m_Entities[InstanceIndex] = Entity;
	m_TextureData.m_TextureHash[InstanceIndex] = texture_hash;
	m_TextureData.m_Size[InstanceIndex] = size;

	m_EntityMap.insert(std::make_pair(Entity, InstanceIndex));

	return SInstanceIndex(InstanceIndex);
}