#include "Transform/VelocityComponentManager.h"
#include <windows.h>


CVelocityComponentManager::CVelocityComponentManager()
{
	Allocate(NUM_VELOCITY_COMPONENTS);
}

CVelocityComponentManager::~CVelocityComponentManager()
{
	VirtualFree(VelocityData.Buffer, VelocityData.NumAllocated, 0);
}

void CVelocityComponentManager::Allocate(const uint32 InSize)
{
	SInstanceData NewData = {};
	const uint32 Bytes = InSize * (sizeof(SEntityId) + sizeof(CVector2f));
	NewData.Buffer = VirtualAlloc(0, Bytes, MEM_COMMIT, PAGE_READWRITE);
	NewData.NumUsed = VelocityData.NumUsed;
	NewData.NumAllocated = InSize;

	NewData.Entities = static_cast<SEntityId*>(NewData.Buffer);
	NewData.Velocity = reinterpret_cast<CVector2f*>(NewData.Entities + InSize);

	memcpy(NewData.Entities, VelocityData.Entities, VelocityData.NumUsed * sizeof(SEntityId));
	memcpy(NewData.Velocity, VelocityData.Velocity, VelocityData.NumUsed * sizeof(CVector2f));

	VirtualFree(VelocityData.Buffer, VelocityData.NumAllocated, 0);

	VelocityData = NewData;
}

const SInstanceIndex CVelocityComponentManager::GetInstance(const SEntityId Entity) const
{
	return EntityMap[Entity];
}

const CVector2f& CVelocityComponentManager::GetVelocity(const SInstanceIndex& Instance) const
{
	return VelocityData.Velocity[Instance];
}

const CVector2f& CVelocityComponentManager::GetVelocity(const SEntityId& Entity) const
{
	const SInstanceIndex InstanceIndex = GetInstance(Entity);
	return VelocityData.Velocity[InstanceIndex];
}

void CVelocityComponentManager::GetVelocity(const SEntityId& Entity, const CVector2f& Velocity) const
{
	const SInstanceIndex InstanceIndex = GetInstance(Entity);
	VelocityData.Velocity[InstanceIndex] = Velocity;
}

CVector2f& CVelocityComponentManager::GetVelocity(const SEntityId& Entity)
{
	return const_cast<CVector2f&>(const_cast<const CVelocityComponentManager*>(this)->GetVelocity(Entity));
}

void CVelocityComponentManager::DestroyInstance(const SInstanceIndex& Instance)
{
	const size_t Last = VelocityData.NumUsed - 1;
	const SEntityId Entity = VelocityData.Entities[Instance];
	const SEntityId LastEntity = VelocityData.Entities[Last];

	VelocityData.Entities[Instance] = VelocityData.Entities[Last];
	VelocityData.Velocity[Instance] = VelocityData.Velocity[Last];

	EntityMap[LastEntity] = Instance;
	EntityMap.erase(Entity);

	--VelocityData.NumUsed;
}

const SInstanceIndex CVelocityComponentManager::CreateInstance(const SEntityId& Entity, const CVector2f& Position)
{
	const uint32 InstanceIndex = VelocityData.NumUsed;
	++VelocityData.NumUsed;

	VelocityData.Entities[InstanceIndex] = Entity;
	VelocityData.Velocity[InstanceIndex] = Position;

	EntityMap.insert(std::make_pair(Entity, InstanceIndex));

	return SInstanceIndex(InstanceIndex);
}

const CVector2f* CVelocityComponentManager::GetInstances(uint32& NumInstances) const
{
	NumInstances = VelocityData.NumUsed;
	return VelocityData.Velocity;
}