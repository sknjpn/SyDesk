#include "MainViewer.h"

MainViewer::Workspace::Shape::Shape(const Polygon& polygon)
	: m_polygon(polygon)
{

}

void MainViewer::Workspace::Shape::update(const RouteGenerator& routeGenerator)
{
	m_cuttingPolygon = m_polygon.calculateRoundBuffer(routeGenerator.m_cuttingMargin);
	m_circlingPolygon = m_cuttingPolygon.calculateRoundBuffer(routeGenerator.m_circlingMargin);

	{
		Array<Vec2> outer;
		outer.emplace_back(m_circlingPolygon.outer().front());

		for (auto& p : m_circlingPolygon.outer())
		{
			if (outer.back().distanceFrom(p) > routeGenerator.m_circlingMargin)
				outer.emplace_back(p);
		}

		m_circlingPolygon = Polygon(outer);
	}
}
