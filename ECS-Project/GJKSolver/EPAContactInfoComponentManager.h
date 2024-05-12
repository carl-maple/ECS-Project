// #pragma once
// 
// #include "Component/Component.h"
// #include "Entity/Entity.h"
// 
// #include <map>
// 
// class CEPAContactInfoComponentManager : public IComponent
// {
// public:
// 	CEPAContactInfoComponentManager();
// 	~CEPAContactInfoComponentManager();
// 
// 	void Allocate(const uint32 size);
// 	const SInstanceIndex GetInstance(const SEntityId& Entity) const;
// 	void DestroyInstance(const SInstanceIndex& Instance);
// 
// 	const SInstanceIndex CreateInstance(const SEntityId& Entity, const size_t texture_hash, const CVector2f& size);
// 
// 	const size_t GetTextureHash(const SInstanceIndex index) const { return m_EPAContactInfo.m_TextureHash[index]; }
// 	const CVector2f& GetSize(const SInstanceIndex index) const { return m_EPAContactInfo.m_Size[index]; }
// 
// private:
// 	constexpr static uint32 NUM_EPA_CONTACT_INFO_COMPONENTS = 20000;
// 	constexpr static uint32 INVALID_InstanceIndex = std::numeric_limits<uint32>::max();
// 
// 	struct SEPAContactInfoComponent
// 	{
// 		SEPAContactInfo m_ContactInfo;
// 		SEntityId m_EntityCollided;
// 		SEPAContactInfoComponent* m_Child = nullptr;
// 	};
// 
// 	struct SInstanceData
// 	{
// 		uint32 m_NumUsed;
// 		uint32 m_NumAllocated;
// 		void* m_Buffer;
// 		SEntityId* m_Entities;
// 		SEPAContactInfoComponent* m_EPAContactInfo;
// 	};
// 
// 	SInstanceData m_EPAContactInfo;
// 
// 	mutable std::map<SEntityId, uint32> m_EntityMap;
// };
// 
