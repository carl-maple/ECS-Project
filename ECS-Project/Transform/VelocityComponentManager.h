#pragma once

#include "Component/Component.h"
#include "Entity/Entity.h"
 
#include <map>
#include <limits.h>

class CVelocityComponentManager : public IComponent
{
public:
	CVelocityComponentManager();
	virtual ~CVelocityComponentManager() override;
	void Allocate(const uint32 size);
	const SInstanceIndex GetInstance(const SEntityId entiy) const;

	const CVector2f& GetVelocity(const SInstanceIndex& Instance) const;
	const CVector2f& GetVelocity(const SEntityId& Entity) const;
	void GetVelocity(const SEntityId& Entity, const CVector2f& Velocity) const;
	CVector2f& GetVelocity(const SEntityId& Entity);
	void DestroyInstance(const SInstanceIndex& Instance);

	const SInstanceIndex CreateInstance(const SEntityId& Entity, const CVector2f& position);
	const CVector2f* GetInstances(uint32& NumInstances) const;
private:

	constexpr static uint32 NUM_VELOCITY_COMPONENTS = 100;
	constexpr static uint32 INVALID_InstanceIndex = std::numeric_limits<uint32>::max();

	struct SInstanceData
	{
		uint32 NumUsed;
		uint32 NumAllocated;
		void* Buffer;
		SEntityId* Entities;
		CVector2f* Velocity;
	};

	SInstanceData VelocityData;

	mutable std::map<SEntityId, uint32> EntityMap;
};

struct SVelocityComponent
{
	CVector2f Velocity;
};
