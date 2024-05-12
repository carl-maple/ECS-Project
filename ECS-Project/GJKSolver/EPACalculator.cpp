#include "EPACalculator.h"

CEPACalculator::CEPACalculator()
{
}


CEPACalculator::~CEPACalculator()
{
}

SEPAContactInfo CEPACalculator::GetContactInfo(const std::vector<CVector2f>& ShapeLhs, const std::vector<CVector2f>& ShapeRhs, std::vector<SSupportPoint>& SupportPoints)
{
	SEPAContactInfo Result;
	SSupportPoint NextSupportPoint;

	if (SupportPoints.size() == 1)
	{
		constexpr uint32 NumberOfSearchDirs = 4;
		const CVector2f SearchDirs[NumberOfSearchDirs] =
		{
			CVector2f(1.0f, 0.0f),
			CVector2f(-1.0f, 0.0f),
			CVector2f(0.0f, 1.0f),
			CVector2f(0.0f, -1.0f)
		};

		for (const CVector2f& SearchDir : SearchDirs)
		{
			NextSupportPoint = SSupportPoint::GetSupportPoint(ShapeLhs, ShapeRhs, SearchDir);

			if ((NextSupportPoint.Position - SupportPoints[0].Position).LengthSq() >= std::numeric_limits<float>::epsilon())
			{
				SupportPoints.emplace_back(NextSupportPoint);
				break;
			}
		}
	}

	if (SupportPoints.size() == 2)
	{
		const CVector2f ABVector = SupportPoints[0].Position - SupportPoints[1].Position;
		const CVector2f SearchDir = CVector2f::TripleProduct(ABVector, -SupportPoints[1].Position, ABVector);

		NextSupportPoint = SSupportPoint::GetSupportPoint(ShapeLhs, ShapeRhs, SearchDir);

		if ((NextSupportPoint.Position - SupportPoints[0].Position).LengthSq() < std::numeric_limits<float>::epsilon())
		{
			NextSupportPoint = SSupportPoint::GetSupportPoint(ShapeLhs, ShapeRhs, -SearchDir);
		}

		SupportPoints.emplace_back(NextSupportPoint);
	}

	SEdge Edge;
	while (true)
	{
		Edge = FindClosesetEdge(SupportPoints);
		NextSupportPoint = SSupportPoint::GetSupportPoint(ShapeLhs, ShapeRhs, Edge.m_Normal);
		const float depth = NextSupportPoint.Position.Dot(Edge.m_Normal);

		if (depth - Edge.m_Distance < std::numeric_limits<float>::epsilon())
		{
			Result.m_Normal = Edge.m_Normal;
			Result.m_Depth = static_cast<int32>(depth);
			break;
		}
		else
		{
			SupportPoints.insert(SupportPoints.begin() + Edge.m_Index + 1, NextSupportPoint);
		}
	}

	const CVector2f line_vector = Edge.m_A - Edge.m_B;
	const CVector2f projection_orientation =  (line_vector / line_vector.LengthSq()) * line_vector.Dot(-Edge.m_B);

	const float s = sqrt(projection_orientation.LengthSq() / line_vector.LengthSq());
	const float inverted_s = 1 - s;

	const uint32 NextLine = (Edge.m_Index + 1) % SupportPoints.size();
	Result.m_PositionA = SupportPoints[Edge.m_Index].m_OrginA + inverted_s * SupportPoints[NextLine].m_OrginA;
	Result.m_PositionB = SupportPoints[Edge.m_Index].m_OrginB + inverted_s * SupportPoints[NextLine].m_OrginB;

	return Result;
}

CEPACalculator::SEdge CEPACalculator::FindClosesetEdge(const std::vector<SSupportPoint>& support_points)
{
	SEdge result;
	uint32 next_line;
	float minDist = std::numeric_limits<float>::max(), currDist;

	CVector2f normal, edge;

	for (uint32 i = 0; i < support_points.size(); ++i)
	{
		next_line = (i + 1) % support_points.size();

		edge = support_points[next_line].Position - support_points[i].Position;
		normal = CVector2f::TripleProduct(edge, support_points[i].Position, edge);
		normal.Normalize();
		currDist = normal.Dot(support_points[i].Position);
		if (currDist < minDist)
		{
			minDist = currDist;
			result.m_B = support_points[i].Position;
			result.m_B = support_points[next_line].Position;
			result.m_Normal = normal;
			result.m_Distance = currDist;
			result.m_Index = i;
		}
	}

	return result;
}