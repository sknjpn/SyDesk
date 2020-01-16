#include "RouteGenerator.h"
#include "Node.h"

std::unique_ptr<RouteGenerator> RouteGenerator::g_routeGenerator;
std::mutex RouteGenerator::g_mutex;

void RouteGenerator::update()
{
	// 初期化
	{
		std::lock_guard<std::mutex> lock(g_mutex);

		m_nodes.clear();
		m_route.clear();
		m_isValid = false;
	}

	// 一時領域確保
	MultiPolygon	temp_cuttingPolygons;
	MultiPolygon	temp_circlingPolygons;
	Vec2			temp_workspaceSize;
	Array<Vec2>		temp_route;
	double	temp_cuttingMargin;
	double	temp_circlingMargin;
	double	temp_cuttingTime;
	double	temp_cuttingSpeed;

	// Copy
	{
		std::lock_guard<std::mutex> lock(g_mutex);

		temp_cuttingPolygons = m_cuttingPolygons;
		temp_circlingPolygons = m_circlingPolygons;
		temp_cuttingMargin = m_cuttingMargin;
		temp_circlingMargin = m_circlingMargin;
		temp_workspaceSize = m_workspaceSize;
		temp_cuttingSpeed = m_cuttingSpeed;
		temp_cuttingTime = m_cuttingTime;
	}

	const int numPolygons = temp_cuttingPolygons.size();

	if (numPolygons == 0) return;

	// ワーク内か判定
	for (const auto& polygon : temp_circlingPolygons)
		if (!RectF(temp_workspaceSize).contains(polygon)) return;

	// 重なり判定
	for (auto it1 = temp_circlingPolygons.begin(); it1 != temp_circlingPolygons.end(); ++it1)
		for (auto it2 = it1 + 1; it2 != temp_circlingPolygons.end(); ++it2)
			if (it1->intersects(*it2)) return;

	{
		MultiPolygon restrictedPolygons;
		for (const auto& polygon : temp_circlingPolygons)
			restrictedPolygons.emplace_back(polygon.calculateBuffer(-0.1));	// それ自体が当たらないようにする

		// 始点Node作成
		{
			const auto& node = m_nodes.emplace_back(MakeShared<Node>());
			node->m_shapeIndex = -1;
			node->m_outerIndex = 0;
			node->m_position = Vec2::Zero();
		}

		// PolygonのNode生成
		for (int polygonIndex = 0; polygonIndex < temp_circlingPolygons.size(); ++polygonIndex)
		{
			for (int outerIndex = 0; outerIndex < temp_circlingPolygons[polygonIndex].outer().size(); ++outerIndex)
			{
				const auto& node = m_nodes.emplace_back(MakeShared<Node>());
				node->m_shapeIndex = polygonIndex;
				node->m_outerIndex = outerIndex;
				node->m_position = m_circlingPolygons[polygonIndex].outer()[outerIndex];
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
		for (int polygonIndex = 0; polygonIndex < numPolygons; ++polygonIndex)
		{
			end = nullptr;

			buildCostMap(start);

			for (const auto& node : m_nodes)
				if (node->m_shapeIndex == polygonIndex && node->m_from && (!end || node->cost < end->cost))
					end = node;

			if (!end) return;

			temp_route.append(getRoute(start, end));

			int minOuterIndex = 0;
			for (int outerIndex = 0; outerIndex < temp_cuttingPolygons[polygonIndex].outer().size(); ++outerIndex)
			{
				if (end->m_position.distanceFrom(temp_cuttingPolygons[polygonIndex].outer()[outerIndex]) <
					end->m_position.distanceFrom(temp_cuttingPolygons[polygonIndex].outer()[minOuterIndex]))
				{
					minOuterIndex = outerIndex;
				}
			}

			for (int outerIndex = minOuterIndex; outerIndex < temp_cuttingPolygons[polygonIndex].outer().size(); ++outerIndex)
				temp_route.emplace_back(temp_cuttingPolygons[polygonIndex].outer()[outerIndex]);

			for (int outerIndex = 0; outerIndex <= minOuterIndex; ++outerIndex)
				temp_route.emplace_back(temp_cuttingPolygons[polygonIndex].outer()[outerIndex]);

			start = end;
		}

		start = end;
		end = m_nodes.front();
		buildCostMap(start);
		temp_route.append(getRoute(start, end));
	}

	// 時間計測
	{
		double length = 0.0;
		for (auto it = temp_route.begin(); it < temp_route.end() - 1; ++it)
			length += (*it - *(it + 1)).length();

		temp_cuttingTime = length / (10000.0 / temp_cuttingSpeed);
	}

	// Copy
	{
		std::lock_guard<std::mutex> lock(g_mutex);

		m_route = temp_route;
		m_isValid = true;
		m_cuttingTime = temp_cuttingTime;
	}
}

Array<Command> RouteGenerator::getCommands() const
{
	std::lock_guard<std::mutex> lock(g_mutex);

	Array<Vec2> simpled;
	simpled.emplace_back(m_route.front());
	double minDistance = 0.25;
	for (const auto& p : m_route)
		if (simpled.back().distanceFrom(p) > minDistance)
			simpled.push_back(p);

	if (simpled.back() != Vec2::Zero()) simpled.emplace_back(Vec2::Zero());

	Array<Command> commands;
	for (int i = 0; i < simpled.size() - 1; ++i)
	{
		const auto& r0 = simpled[i];
		const auto& r1 = simpled[i + 1];
		const double length = (r0 - r1).length();

		Point p0 = (r0 * 100.0).asPoint();
		Point p1 = (r1 * 100.0).asPoint();
		double sx = Min(30000.0, Abs(m_cuttingSpeed * length / (r1 - r0).x));
		double sy = Min(30000.0, Abs(m_cuttingSpeed * length / (r1 - r0).y));

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
