#pragma once
#include <map>
#include <typeindex>
#include "SystemType.h"

class CSystemTypeManager
{
public:
	CSystemTypeManager();
	~CSystemTypeManager();

	const CSystemType& GetType(const std::type_index& type_index);

	template<typename T>
	const CSystemType& GetType()
	{
		return GetType(static_cast<typeid>(T));
	}

	template<typename T>
	const uint32& GetId()
	{
		return GetType(static_cast<typeid>(T)).GetId();
	}

	template<typename T>
	const std::bitset<NBitSize::BIT_SIZE>& GetBit()
	{
		return GetType(typeid(T)).GetBit();
	}
private:
	std::map<std::type_index, CSystemType*> m_SystemTypeMap;
};

