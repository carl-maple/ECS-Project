#include "AABoxComponentManager.h"
#include <windows.h>
#include "AABox.h"

CAABoxComponentManager::CAABoxComponentManager()
{
	Allocate(NUM_BOX_COMPONENTS);
}

CAABoxComponentManager::~CAABoxComponentManager()
{
	VirtualFree(BoxData.Buffer, BoxData.NumAllocated, 0);
}

void CAABoxComponentManager::Allocate(const uint32 size)
{
	SInstanceData NewData = {};
	const uint32 Bytes = size * (sizeof(SEntityId) + sizeof(CAABox));
	NewData.Buffer = VirtualAlloc(0, Bytes, MEM_COMMIT, PAGE_READWRITE);
	NewData.NumUsed = BoxData.NumUsed;
	NewData.NumAllocated = size;

	NewData.Entities = static_cast<SEntityId*>(NewData.Buffer);
	NewData.Box = reinterpret_cast<CAABox*>(NewData.Entities + size);

	memcpy(NewData.Entities, BoxData.Entities, BoxData.NumUsed * sizeof(SEntityId));
	memcpy(NewData.Box, BoxData.Box, BoxData.NumUsed * sizeof(CAABox));

	VirtualFree(BoxData.Buffer, BoxData.NumAllocated, 0);

	BoxData = NewData;
}

const SInstanceIndex CAABoxComponentManager::GetInstance(const SEntityId entiy) const
{
	return EntityMap[entiy];
}

const CAABox& CAABoxComponentManager::GetBox(const SInstanceIndex& Instance) const
{
	return BoxData.Box[Instance];
}

const CAABox& CAABoxComponentManager::GetBox(const SEntityId& Entity) const
{
	const SInstanceIndex InstanceIndex = GetInstance(Entity);
	return BoxData.Box[InstanceIndex];
}

void CAABoxComponentManager::SetBox(const SEntityId& Entity, const CAABox& InBox) const
{
	const SInstanceIndex InstanceIndex = GetInstance(Entity);
	BoxData.Box[InstanceIndex] = InBox;
}

CAABox& CAABoxComponentManager::GetBox(const SEntityId& Entity)
{
	return const_cast<CAABox&>(const_cast<const CAABoxComponentManager*>(this)->GetBox(Entity));
}

void CAABoxComponentManager::DestroyInstance(const SInstanceIndex& Instance)
{
	const size_t Last = BoxData.NumUsed - 1;
	const SEntityId Entity = BoxData.Entities[Instance];
	const SEntityId LastEntity = BoxData.Entities[Last];

	BoxData.Entities[Instance] = BoxData.Entities[Last];
	BoxData.Box[Instance] = BoxData.Box[Last];

	EntityMap[LastEntity] = Instance;
	EntityMap.erase(Entity);

	--BoxData.NumUsed;
}

const SInstanceIndex CAABoxComponentManager::CreateInstance(const SEntityId& Entity, const CAABox& InBox)
{
	const uint32 InstanceIndex = BoxData.NumUsed;
	++BoxData.NumUsed;

	BoxData.Entities[InstanceIndex] = Entity;
	BoxData.Box[InstanceIndex] = InBox;

	EntityMap.insert(std::make_pair(Entity, InstanceIndex));

	return SInstanceIndex(InstanceIndex);
}

const CAABox* CAABoxComponentManager::GetInstances(uint32& NumInstances) const
{
	NumInstances = BoxData.NumUsed;
	return BoxData.Box;
}