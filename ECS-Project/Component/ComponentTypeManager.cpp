#include "Component/ComponentTypeManager.h"

CComponentTypeManager::CComponentTypeManager()
{
}


CComponentTypeManager::~CComponentTypeManager()
{
}

const CComponentType& CComponentTypeManager::GetType(const std::type_index& type_index)
{
	CComponentType* component_type = m_ComponentTypeMap[type_index];
	if (component_type == nullptr)
	{
		component_type = new CComponentType();
		m_ComponentTypeMap[type_index] = component_type;
	}

	return *component_type;
}
