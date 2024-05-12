#include "Types/Types.h"
#include "Color.h"

#include <string.h>

namespace NDebugRender
{
	struct SLineStruct
	{
		SLineStruct(const CVector2f& InStart, const CVector2f& InEnd, const NColor::SColor& InColor)
			:Start(InStart), End(InEnd), Color(InColor) {}

		CVector2f Start;
		CVector2f End;
		NColor::SColor Color;
	};

	struct SDebugText
	{
		SDebugText(std::string&& InDebugText, const CVector2f& InPosition, const NColor::SColor& InColor)
			:DebugText(InDebugText), Position(InPosition), Color(InColor) {}

		std::string DebugText;
		CVector2f Position;
		NColor::SColor Color;
	};
}
