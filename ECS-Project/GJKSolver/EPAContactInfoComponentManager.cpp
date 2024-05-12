// #include "EPAContactInfoComponentManager.h"
// #include <windows.h>
// #include "EPAContactInfo.h"
// 
// CEPAContactInfoComponentManager::CEPAContactInfoComponentManager()
// {
// 	Allocate(NUM_EPA_CONTACT_INFO_COMPONENTS);
// }
// 
// 
// CEPAContactInfoComponentManager::~CEPAContactInfoComponentManager()
// {
// 	VirtualFree(m_EPAContactInfo.m_Buffer, m_EPAContactInfo.m_NumAllocated, 0);
// }
// 
// void CEPAContactInfoComponentManager::Allocate(const uint32 size)
// {
// 	SInstanceData new_data = {};
// 	const uint32 bytes = size * (sizeof(SEntityId) + sizeof(size_t) + sizeof(SEPAContactInfo));
// 	new_data.m_Buffer = VirtualAlloc(0, bytes, MEM_COMMIT, PAGE_READWRITE);
// 	new_data.m_NumUsed = m_EPAContactInfo.m_NumUsed;
// 	new_data.m_NumAllocated = size;
// 
// 	new_data.m_Entities = static_cast<SEntityId*>(new_data.m_Buffer);
// 	new_data.m_TextureHash = reinterpret_cast<size_t*>(new_data.m_Entities + size);
// 	new_data.m_Size = reinterpret_cast<SEPAContactInfo*>(new_data.m_TextureHash + size);
// 
// 	memcpy(new_data.m_Entities, m_EPAContactInfo.m_Entities, m_EPAContactInfo.m_NumUsed * sizeof(SEntityId));
// 	memcpy(new_data.m_TextureHash, m_EPAContactInfo.m_TextureHash, m_EPAContactInfo.m_NumUsed * sizeof(size_t));
// 	memcpy(new_data.m_Size, m_EPAContactInfo.m_Size, m_EPAContactInfo.m_NumUsed * sizeof(SEPAContactInfo));
// 
// 	VirtualFree(m_EPAContactInfo.m_Buffer, m_EPAContactInfo.m_NumAllocated, 0);
// 
// 	m_EPAContactInfo = new_data;
// }
// 
// const SInstanceIndex CEPAContactInfoComponentManager::GetInstance(const SEntityId& Entity) const
// {
// 	return m_EntityMap[Entity];
// }
// 
// void CEPAContactInfoComponentManager::DestroyInstance(const SInstanceIndex& Instance)
// {
// 	const size_t last = m_EPAContactInfo.m_NumUsed - 1;
// 	const SEntityId Entity = m_EPAContactInfo.m_Entities[Instance];
// 	const SEntityId LastEntity = m_EPAContactInfo.m_Entities[last];
// 
// 	m_EPAContactInfo.m_Entities[Instance] = m_EPAContactInfo.m_Entities[last];
// 	m_EPAContactInfo.m_TextureHash[Instance] = m_EPAContactInfo.m_TextureHash[last];
// 	m_EPAContactInfo.m_Size[Instance] = m_EPAContactInfo.m_Size[last];
// 
// 	m_EntityMap[LastEntity] = Instance;
// 	m_EntityMap.erase(Entity);
// 
// 	--m_EPAContactInfo.m_NumUsed;
// }
// 
// const SInstanceIndex CEPAContactInfoComponentManager::CreateInstance(const SEntityId& Entity, const size_t texture_hash, const CVector2f& size)
// {
// 	const uint32 InstanceIndex = m_EPAContactInfo.m_NumUsed;
// 	++m_EPAContactInfo.m_NumUsed;
// 
// 	m_EPAContactInfo.m_Entities[InstanceIndex] = Entity;
// 	m_EPAContactInfo.m_TextureHash[InstanceIndex] = texture_hash;
// 	m_EPAContactInfo.m_Size[InstanceIndex] = size;
// 
// 	m_EntityMap.insert(std::make_pair(Entity, InstanceIndex));
// 
// 	return SInstanceIndex(InstanceIndex);
// }