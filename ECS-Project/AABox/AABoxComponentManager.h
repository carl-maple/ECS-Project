#pragma once

#include "Component/Component.h"
#include "Entity/Entity.h"

#include <map>
#include <limits.h>

class CAABox;

class CAABoxComponentManager : public IComponent
{
public:
	CAABoxComponentManager();
	virtual ~CAABoxComponentManager() override;
	void Allocate(const uint32 size);
	const SInstanceIndex GetInstance(const SEntityId entiy) const;

	const CAABox& GetBox(const SInstanceIndex& Instance) const;
	const CAABox& GetBox(const SEntityId& Entity) const;
	void SetBox(const SEntityId& Entity, const CAABox& box) const;
	CAABox& GetBox(const SEntityId& Entity);
	void DestroyInstance(const SInstanceIndex& Instance);

	const SInstanceIndex CreateInstance(const SEntityId& Entity, const CAABox& box);
	const CAABox* GetInstances(uint32& NumInstances) const;
private:

	constexpr static uint32 NUM_BOX_COMPONENTS = 100;
	constexpr static uint32 INVALID_InstanceIndex = std::numeric_limits<uint32>::max();

	struct SInstanceData
	{
		uint32 NumUsed;
		uint32 NumAllocated;
		void* Buffer;
		SEntityId* Entities;
		CAABox* Box;
	};

	SInstanceData BoxData;

	mutable std::map<SEntityId, uint32> EntityMap;
};

