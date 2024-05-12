// #pragma once
// #include <map>
// #include "Base.h"
// 
// class CSpatialMapping
// {
// public:
// 	CSpatialMapping(const uint32 hash_table_size);
// 	~CSpatialMapping();
// 	void InsertGameObject(const std::shared_ptr<CGameObject>& game_object);
// private:
// 	const CVector2i GetHash(const CVector2f& point) const;
// 	typedef std::vector < std::weak_ptr<CGameObject> > GameObjectWeakPtrVector;
// 	std::map<int, std::map< int, GameObjectWeakPtrVector>> m_HashTable;
// 	uint32 m_HashTableSize;
// };

