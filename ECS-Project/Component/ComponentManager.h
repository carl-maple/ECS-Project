#pragma once

#include "Component/ComponentTypeManager.h"
#include "World/World.h"
#include "Entity/Entity.h"
#include "Component/Component.h"

#include <vector>
#include <map>

class IComponent;

template <class TComponent>
class CComponentPool : public IComponent
{
public:
	CComponentPool(const size_t num_to_allocate);
	const SInstanceIndex CreateInstance(const SEntityId& Entity, TComponent&& component);
	void DestroyInstance(const SInstanceIndex& Instance);
	const SInstanceIndex GetInstance(const SEntityId& Entity) const;
	TComponent& GetComponent(const SEntityId& Entity);
	const TComponent& GetComponent(const SEntityId& Entity) const;
private:
	std::vector<TComponent> Buffer;
	mutable std::map<SEntityId, uint32> EntityMap;
	uint32 NumUsed = 0;
};

template <class TComponent>
const TComponent& CComponentPool<TComponent>::GetComponent(const SEntityId& Entity) const
{
	return Buffer[GetInstance(Entity)];
}

template <class TComponent>
TComponent& CComponentPool<TComponent>::GetComponent(const SEntityId& Entity)
{
	return const_cast<TComponent&>(const_cast<const CComponentPool<TComponent>*>(this)->GetComponent(Entity));
}

template <class TComponent>
const SInstanceIndex CComponentPool<TComponent>::GetInstance(const SEntityId& Entity) const
{
	return EntityMap[Entity];
}

template <class TComponent>
void CComponentPool<TComponent>::DestroyInstance(const SInstanceIndex& Instance)
{
	const size_t last = NumUsed - 1;
	const SEntityId Entity = m_Entities[Instance];
	const SEntityId LastEntity = m_Entities[last];

	Buffer[Instance] = Buffer[last];
	EntityMap[LastEntity] = Instance;
	EntityMap.erase(Entity);

	--NumUsed;
}

template <class TComponent>
const SInstanceIndex CComponentPool<TComponent>::CreateInstance(const SEntityId& Entity, TComponent&& InComponent)
{
	const uint32 InstanceIndex = NumUsed;
	++NumUsed;

	Buffer[InstanceIndex] = InComponent;
	EntityMap.insert(std::make_pair(Entity, InstanceIndex));

	return SInstanceIndex(InstanceIndex);
}

template <class TComponent>
CComponentPool<TComponent>::CComponentPool(const size_t NumberToAllocate)
{
	Buffer.resize(NumberToAllocate);
}

class CComponentManager
{
public:
	CComponentManager(CWorld* InWorld);
	~CComponentManager();

	template<typename T>
	CComponentPool<T>* GetComponent()
	{
		const CComponentType& ComponentType = World->GetComponentTypeManager()->GetType(typeid(T));
		return static_cast<CComponentPool<T>*>(Components[ComponentType.GetId()].get());
	}

private:

	template<typename T>
	void CreateComponent(const size_t NumberToAllocate)
	{
		const CComponentType& ComponentType = World->GetComponentTypeManager()->GetType(typeid(T));
		Components[ComponentType.GetId()] = std::make_unique<CComponentPool<T>>(NumberToAllocate);
	}

	std::map<uint32, std::unique_ptr<IComponent>> Components;

	CWorld* World;
};

