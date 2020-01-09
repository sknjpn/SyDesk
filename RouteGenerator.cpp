#include "RouteGenerator.h"

void RouteGenerator::updateRoute()
{
	m_route.clear();

	for (const auto& polygon : m_multiPolygon)
	{
		m_cuttingMultiPolygons.emplace_back(polygon.calculateRoundBuffer(m_cuttingMargin));
		m_circlingMultiPolygons.emplace_back(polygon.calculateRoundBuffer(m_circlingMargin));
	}

}

void RouteGenerator::updateValid()
{
	m_isValid = false;

	if (m_circlingMargin < 0 || m_cuttingMargin < 0) return;



	// ワーク内か判定
	for (const auto& polygon : m_circlingMultiPolygons)
		if (!RectF(m_workspaceSize).contains(polygon)) return;

	// 重なり判定
	for (auto it1 = m_cuttingMultiPolygons.begin(); it1 != m_cuttingMultiPolygons.end(); ++it1)
		for (auto it2 = it1 + 1; it2 != m_cuttingMultiPolygons.end(); ++it2)
			if (it1->intersects(*it2)) return;

	// 重なり判定
	for (auto it1 = m_circlingMultiPolygons.begin(); it1 != m_circlingMultiPolygons.end(); ++it1)
		for (auto it2 = it1 + 1; it2 != m_circlingMultiPolygons.end(); ++it2)
			if (it1->intersects(*it2)) return;


	m_isValid = true;
}

void RouteGenerator::update()
{
	m_cuttingMultiPolygons.clear();
	m_circlingMultiPolygons.clear();
	m_route.clear();

	for (const auto& polygon : m_multiPolygon)
	{
		m_cuttingMultiPolygons.emplace_back(polygon.calculateRoundBuffer(m_cuttingMargin));
		m_circlingMultiPolygons.emplace_back(polygon.calculateRoundBuffer(m_circlingMargin));
	}

	for (auto& polygon : m_cuttingMultiPolygons)
		polygon = polygon.simplified(m_cuttingInterval);

	for (auto& polygon : m_circlingMultiPolygons)
		polygon = polygon.simplified(m_circlingInterval);

	updateValid();

	if (m_isValid) updateRoute();
}

Array<Command> RouteGenerator::getCommands() const
{
	Array<Vec2>	m_route;

	for (const auto& polygon : m_multiPolygon)
		m_route.append(polygon.outer());

	Array<Command> commands;
	for (const auto& p : m_route)
	{
		commands.emplace_back('P', short(p.x * 10), short(2000), short(p.y * 10), short(2000));
	}

	return commands;
}
