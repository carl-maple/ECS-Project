#pragma once
#include <map>
#include <typeindex>
#include "Component/ComponentType.h"

class CComponentTypeManager
{
public:
	CComponentTypeManager();
	~CComponentTypeManager();

	const CComponentType& GetType(const std::type_index& type_index);

	template<typename T>
	const CComponentType& GetType()
	{
		return GetType(static_cast<typeid>(T));
	}

	template<typename T>
	const uint32& GetId()
	{
		return GetType(static_cast<typeid>(T)).GetId();
	}

	template<typename T>
	const uint32& GetBit()
	{
		return GetType(static_cast<typeid>(T)).GetBit();
	}
private:
	std::map<std::type_index, CComponentType*> m_ComponentTypeMap;
};

