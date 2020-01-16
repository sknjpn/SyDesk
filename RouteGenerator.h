#pragma once

#include "common.h"
#include "Command.h"
#include <mutex>

class Node;


class RouteGenerator
{
	static std::unique_ptr<RouteGenerator> g_routeGenerator;
	static std::mutex g_mutex;

	Array<Vec2>		m_route;

	// used by only update
	Array<std::shared_ptr<Node>> m_nodes;

	bool m_isInUpdate = false;
	bool m_isNeedUpdate = true;

	MultiPolygon	m_cuttingPolygons;
	MultiPolygon	m_circlingPolygons;
	Vec2			m_workspaceSize;

	double	m_cuttingMargin;
	double	m_circlingMargin;
	double	m_cuttingSpeed;

	void	buildCostMap(const std::shared_ptr<Node>& start);
	Array<Vec2> getRoute(const std::shared_ptr<Node>& start, const std::shared_ptr<Node>& end) const;

	bool	m_isValid = false;

public:
	RouteGenerator()
		: m_cuttingMargin(0.0)
		, m_circlingMargin(0.0)
		, m_cuttingSpeed(1500)
		, m_workspaceSize(182 - 10, 128 - 10)
	{}

	static void MakeInstance() { g_routeGenerator = MakeUnique<RouteGenerator>(); }
	static const std::unique_ptr<RouteGenerator>& GetInstance() { return g_routeGenerator; }

	void	update();

	bool	isValid() const { std::lock_guard<std::mutex> lock(g_mutex); return m_isValid; }

	// Set
	void	setWorkspaceSize(const Vec2& workspaceSize) { std::lock_guard<std::mutex> lock(g_mutex); m_workspaceSize = workspaceSize; m_isNeedUpdate = true; }
	void	setCuttingMargin(const double margin) { std::lock_guard<std::mutex> lock(g_mutex); m_cuttingMargin = margin; }
	void	setCirclingMargin(const double margin) { std::lock_guard<std::mutex> lock(g_mutex); m_circlingMargin = margin; }
	void	setCuttingSpeed(const double speed) { std::lock_guard<std::mutex> lock(g_mutex); m_cuttingSpeed = speed; }
	void	setCuttingPolygons(const MultiPolygon& polygons) { std::lock_guard<std::mutex> lock(g_mutex); m_cuttingPolygons = polygons; }
	void	setCirclingPolygons(const MultiPolygon& polygons) { std::lock_guard<std::mutex> lock(g_mutex); m_circlingPolygons = polygons; }

	// Get
	Array<Vec2> getRoute() const { std::lock_guard<std::mutex> lock(g_mutex); return m_route; }
	double	getCuttingMargin() const { std::lock_guard<std::mutex> lock(g_mutex); return m_cuttingMargin; }
	double	getCirclingMargin() const { std::lock_guard<std::mutex> lock(g_mutex); return m_circlingMargin; }
	Vec2	getWorkspaceSize() const { return m_workspaceSize; }

	Array<Command>	getCommands() const;
};
