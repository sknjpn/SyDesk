#include "RouteGenerator.h"
#include "Node.h"

std::mutex g_routeGeneratorMutex;

void RouteGenerator::update()
{
	m_nodes.clear();
	m_route.clear();
	m_isValid = false;

	if (m_circlingMultiPolygons.isEmpty() || m_cuttingMultiPolygons.isEmpty()) return;

	// ワーク内か判定
	for (const auto& polygon : m_circlingMultiPolygons)
		if (!RectF(m_workspaceSize).contains(polygon)) return;

	// 重なり判定
	for (auto it1 = m_circlingMultiPolygons.begin(); it1 != m_circlingMultiPolygons.end(); ++it1)
		for (auto it2 = it1 + 1; it2 != m_circlingMultiPolygons.end(); ++it2)
			if (it1->intersects(*it2)) return;

	{
		MultiPolygon restrictedPolygons;
		for (const auto& polygon : m_circlingMultiPolygons)
			restrictedPolygons.emplace_back(polygon.calculateBuffer(-0.1));	// それ自体が当たらないようにする

		// 始点Node作成
		{
			const auto& node = m_nodes.emplace_back(MakeShared<Node>());
			node->m_shapeIndex = -1;
			node->m_outerIndex = 0;
			node->m_position = Vec2::Zero();
		}

		// PolygonのNode生成
		for (int polygonIndex = 0; polygonIndex < m_circlingMultiPolygons.size(); ++polygonIndex)
		{
			for (int outerIndex = 0; outerIndex < m_circlingMultiPolygons[polygonIndex].outer().size(); ++outerIndex)
			{
				const auto& node = m_nodes.emplace_back(MakeShared<Node>());
				node->m_shapeIndex = polygonIndex;
				node->m_outerIndex = outerIndex;
				node->m_position = m_circlingMultiPolygons[polygonIndex].outer()[outerIndex];
			}
		}

		// 接続形成
		for (auto it1 = m_nodes.begin(); it1 != m_nodes.end(); ++it1)
		{
			for (auto it2 = it1 + 1; it2 != m_nodes.end(); ++it2)
			{
				bool isIntersect = false;
				for (const auto& polygon : restrictedPolygons)
				{
					if (Line((*it1)->m_position, (*it2)->m_position).intersects(polygon))
					{
						isIntersect = true;

						break;
					}
				}

				if (!isIntersect)
				{
					(*it1)->m_connectedNodes.emplace_back(*it2);
					(*it2)->m_connectedNodes.emplace_back(*it1);
				}
			}
		}

		// ビルド
		std::shared_ptr<Node> start = m_nodes.front();
		std::shared_ptr<Node> end;
		for (int polygonIndex = 0; polygonIndex < m_circlingMultiPolygons.size(); ++polygonIndex)
		{
			end = nullptr;

			buildCostMap(start);

			for (const auto& node : m_nodes)
				if (node->m_shapeIndex == polygonIndex && node->m_from && (!end || node->cost < end->cost))
					end = node;

			if (!end) return;

			m_route.append(getRoute(start, end));

			int minOuterIndex = 0;
			for (int outerIndex = 0; outerIndex < m_cuttingMultiPolygons[polygonIndex].outer().size(); ++outerIndex)
			{
				if (end->m_position.distanceFrom(m_cuttingMultiPolygons[polygonIndex].outer()[outerIndex]) <
					end->m_position.distanceFrom(m_cuttingMultiPolygons[polygonIndex].outer()[minOuterIndex]))
				{
					minOuterIndex = outerIndex;
				}
			}

			for (int outerIndex = minOuterIndex; outerIndex < m_cuttingMultiPolygons[polygonIndex].outer().size(); ++outerIndex)
				m_route.emplace_back(m_cuttingMultiPolygons[polygonIndex].outer()[outerIndex]);

			for (int outerIndex = 0; outerIndex <= minOuterIndex; ++outerIndex)
				m_route.emplace_back(m_cuttingMultiPolygons[polygonIndex].outer()[outerIndex]);

			start = end;
		}

		start = end;
		end = m_nodes.front();
		buildCostMap(start);
		m_route.append(getRoute(start, end));
	}

	m_isValid = true;
}

Array<Command> RouteGenerator::getCommands() const
{
	Array<Vec2>	route;
	route.emplace_back(Vec2::Zero());
	route.emplace_back(Vec2::Zero());
	double minLength = 0.25;
	for (const auto& p : m_route)
		if (route.back().distanceFrom(p) > minLength) route.emplace_back(p);

	route.emplace_back(Vec2::Zero());

	Window::SetTitle(route.size());

	Array<Command> commands;
	for (int i = 0; i < route.size() - 1; ++i)
	{
		const auto& r0 = route[i];
		const auto& r1 = route[i + 1];
		const double length = (r0 - r1).length();

		Point p0 = (r0 * 100.0).asPoint();
		Point p1 = (r1 * 100.0).asPoint();
		double sx = Abs(m_cuttingSpeed * length / (r1 - r0).x);
		double sy = Abs(m_cuttingSpeed * length / (r1 - r0).y);

		commands.emplace_back('P', short(p1.x), short(sx), short(p1.y), short(sy));
	}

	commands.emplace_back('K', short(0), short(1500), short(0), short(0));

	return commands;
}

void RouteGenerator::buildCostMap(const std::shared_ptr<Node>& start)
{
	for (const auto& node : m_nodes)
	{
		node->m_from = nullptr;
		node->cost = 0.0;
	}

	Array<std::shared_ptr<Node>> temp;
	temp.emplace_back(start);

	for (int i = 0; i < temp.size(); ++i)
	{
		const auto fromNode = temp[i];
		for (const auto& toNode : fromNode->m_connectedNodes)
		{
			if (toNode != start && (!toNode->m_from || toNode->cost > fromNode->cost + (toNode->m_position - fromNode->m_position).length()))
			{
				toNode->m_from = fromNode;
				toNode->cost = fromNode->cost + (toNode->m_position - fromNode->m_position).length();

				temp.emplace_back(toNode);
			}
		}
	}
}

Array<Vec2> RouteGenerator::getRoute(const std::shared_ptr<Node>& start, const std::shared_ptr<Node>& end) const
{
	Array<Vec2> route;

	std::shared_ptr<Node> now = end;

	for (; now;)
	{
		route.emplace_back(now->m_position);
		now = now->m_from;
	}

	return route.reversed();
}
