#include "SystemTypeManager.h"

CSystemTypeManager::CSystemTypeManager()
{
}


CSystemTypeManager::~CSystemTypeManager()
{
}

const CSystemType& CSystemTypeManager::GetType(const std::type_index& type_index)
{
	CSystemType* component_type = m_SystemTypeMap[type_index];
	if (component_type == nullptr)
	{
		component_type = new CSystemType();
		m_SystemTypeMap[type_index] = component_type;
	}

	return *component_type;
}
