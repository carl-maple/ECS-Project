// #include "SpatialMapping.h"
// #include "AABox.h"
// 
// 
// CSpatialMapping::CSpatialMapping(const uint32 hash_table_size)
// :m_HashTableSize(hash_table_size)
// {
// }
// 
// 
// CSpatialMapping::~CSpatialMapping()
// {
// }
// 
// const CVector2i CSpatialMapping::GetHash(const CVector2f& point) const
// {
// 	const int x_pos = static_cast<int>(point.x / m_HashTableSize);
// 	const int y_pos = static_cast<int>(point.y / m_HashTableSize);
// 	return CVector2i(x_pos, y_pos);
// }
// 
// void CSpatialMapping::InsertGameObject(const std::shared_ptr<CGameObject>& game_object)
// {
// 	const CVector2f& position = game_object->GetPosition();
// 
// 	constexpr static float temp_size = 20.f;
// 	const CAABox box = CAABox::CreateAABoxFromPositionSize(position, CVector2f(temp_size, temp_size));
// 
// 	const CVector2i min_hash = GetHash(box.m_Min);
// 	const CVector2i max_hash = GetHash(box.m_Max);
// 
// 	for (int x = min_hash.x; x < max_hash.x + 1; ++x)
// 	{
// 		for (int y = min_hash.y; x < max_hash.x + 1; ++y)
// 		{
// 			m_HashTable[x][y].push_back(game_object);
// 		}
// 	}
// }