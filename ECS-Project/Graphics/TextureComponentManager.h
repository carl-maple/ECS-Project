#pragma once

#include "Component/Component.h"
#include "Entity/Entity.h"

#include <map>

class CTextureComponentManager : public IComponent
{
public:
	CTextureComponentManager();
	~CTextureComponentManager();

	void Allocate(const uint32 size);
	const SInstanceIndex GetInstance(const SEntityId& Entity) const;
	void DestroyInstance(const SInstanceIndex& Instance);

	const SInstanceIndex CreateInstance(const SEntityId& Entity, const size_t texture_hash, const CVector2f& size);

	const size_t GetTextureHash(const SInstanceIndex index) const { return m_TextureData.m_TextureHash[index]; }
	const CVector2f& GetSize(const SInstanceIndex index) const { return m_TextureData.m_Size[index]; }

private:
	constexpr static uint32 NUM_TEXTURE_COMPONENTS = 20000;
	constexpr static uint32 INVALID_InstanceIndex = std::numeric_limits<uint32>::max();

	struct SInstanceData
	{
		uint32 m_NumUsed;
		uint32 m_NumAllocated;
		void* Buffer;
		SEntityId* m_Entities;
		size_t* m_TextureHash;
		CVector2f* m_Size;
	};

	SInstanceData m_TextureData;

	mutable std::map<SEntityId, uint32> m_EntityMap;
};

struct STextureComponent
{
	size_t TextureHash;
	CVector2f Size;
};