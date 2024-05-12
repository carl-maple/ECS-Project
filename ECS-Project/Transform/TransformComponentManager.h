#pragma once

#include "Component/Component.h"
#include "Entity/Entity.h"

#include <map>
#include <limits.h>

class CTransformComponentManager : public IComponent
{
public:
	CTransformComponentManager();
	virtual ~CTransformComponentManager() override;
	void Allocate(const uint32 size);
	const SInstanceIndex GetInstance(const SEntityId& entiy) const;

	const CVector2f& GetPosition(const SInstanceIndex& Instance) const;
	const CVector2f& GetPosition(const SEntityId& Entity) const;
	CVector2f& GetPosition(const SEntityId& Entity);
	void SetPosition(const SEntityId& Entity, const CVector2f& position);
	void DestroyInstance(const SInstanceIndex& Instance);
	const SInstanceIndex CreateInstance(const SEntityId& Entity, const CVector2f& position);
	const CVector2f* GetInstances(uint32& NumInstances) const;
private:

	constexpr static uint32 NUM_TRANSFORM_COMPONENTS = 20000;
	constexpr static uint32 INVALID_InstanceIndex = std::numeric_limits<uint32>::max();

	struct SInstanceData
	{
		uint32 m_NumUsed;
		uint32 m_NumAllocated;
		void* Buffer;
		SEntityId* m_Entities;
		CVector2f* m_Position;
	};

	SInstanceData m_TransformData;

	mutable std::map<SEntityId, uint32> m_EntityMap;
};

struct STransformComponent
{
	CVector2f Position;
};