#pragma once

#include "Utils/Base.h"
#include "System/System.h"
#include "World/World.h"
#include "SystemTypeManager.h"
#include "Component/GetterUtils.h"

#include <map>

class CWorld;

class CSystemManager
{
public:
	CSystemManager(CWorld* InWorld);
	~CSystemManager();

	template<typename T>
	T* const GetSystem()
	{
		const CSystemType& SystemType = World->GetSystemTypeManager()->GetType(typeid(T));
		return static_cast<T*>(Systems[SystemType.GetId()].get());
	}

	void Update(float DeltaTime);

	CWorld* GetWorld() const { return World; }

	template<typename T>
	const std::bitset<NBitSize::BIT_SIZE>& GetBit()
	{
		return GetWorld()->GetSystemTypeManager()->GetBit<T>();
	}

	template<typename T>
	CComponentPool<T>* GetComponent()
	{
		return NGetterUtils::GetComponent<T>(World);
	}

private:

	template<typename T>
	void CreateSystem()
	{
		const CSystemType& SystemType = World->GetSystemTypeManager()->GetType(typeid(T));
		Systems[SystemType.GetId()] = std::make_unique<T>();
		Systems[SystemType.GetId()]->Init(this);
	}

	std::map<uint32, std::unique_ptr<CSystem>> Systems;
	CWorld* World;
};

