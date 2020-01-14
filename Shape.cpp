#include "Shape.h"

std::mutex g_shapeMutex;

void Shape::update(const RouteGenerator& routeGenerator)
{
	double circlingMargin;
	double cuttingMargin;

	// データコピー
	{
		std::lock_guard<std::mutex> lock(g_routeGeneratorMutex);

		cuttingMargin = routeGenerator.m_cuttingMargin;
		circlingMargin = routeGenerator.m_circlingMargin;
	}

	// バッファ計算
	m_cuttingPolygonTemp = m_polygon.calculateRoundBuffer(cuttingMargin);
	m_circlingPolygonTemp = m_cuttingPolygonTemp.calculateRoundBuffer(circlingMargin);

	// 最小距離の設定
	{
		Array<Vec2> outer;
		outer.emplace_back(m_circlingPolygonTemp.outer().front());

		for (auto& p : m_circlingPolygonTemp.outer())
			if (outer.back().distanceFrom(p) > circlingMargin)
				outer.emplace_back(p);

		m_circlingPolygonTemp = Polygon(outer);
	}

	// データコピー
	{
		std::lock_guard<std::mutex> lock(g_shapeMutex);

		m_cuttingPolygon = m_cuttingPolygonTemp;
		m_circlingPolygon = m_circlingPolygonTemp;
	}
}
