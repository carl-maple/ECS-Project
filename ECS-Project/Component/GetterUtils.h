#pragma once

#include "Component/ComponentManager.h"

namespace NGetterUtils
{
	template<typename T>
	CComponentPool<T>* const GetComponent(CWorld* World)
	{
		return World->GetComponentManager()->GetComponent<T>();
	}
};
