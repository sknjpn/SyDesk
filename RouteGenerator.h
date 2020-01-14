#pragma once

#include "common.h"
#include "Command.h"
#include <mutex>

class Node;

extern std::mutex g_routeGeneratorMutex;

class RouteGenerator
{
	Array<Vec2>		m_route;
	bool			m_isValid;

public:
	bool m_isInUpdate = false;
	bool m_isNeedUpdate = true;

	MultiPolygon	m_cuttingMultiPolygons;
	MultiPolygon	m_circlingMultiPolygons;
	Vec2			m_workspaceSize;

	double	m_cuttingMargin;
	double	m_cuttingInterval;
	double	m_circlingMargin;
	double	m_circlingInterval;

	double	m_cuttingSpeed;
	double	m_wireTemperature;

	Array<std::shared_ptr<Node>> m_nodes;

public:
	RouteGenerator()
		: m_cuttingMargin(0.0)
		, m_circlingMargin(0.0)
		, m_cuttingInterval(0.5)
		, m_circlingInterval(m_circlingMargin)
		, m_cuttingSpeed(1500)
		, m_wireTemperature(25)
		, m_workspaceSize(182 - 10, 128 - 10)
		, m_isValid(true)
	{}

	void	update();

	Array<Command>	getCommands() const;

	bool	isValid() const { return m_isValid; }

	void	setWorkspaceSize(const Vec2& workspaceSize) { m_workspaceSize = workspaceSize; update(); }
	void	setCuttingMargin(const double margin) { m_cuttingMargin = margin; update(); }
	void	setCirclingMargin(const double margin) { m_circlingMargin = margin; m_circlingInterval = m_circlingMargin; update(); }
	//void	setCuttingInterval(const double interval) { m_cuttingInterval = interval; update(); }
	//void	setCirclingInterval(const double interval) { m_circlingInterval = interval; update(); }

	void	buildCostMap(const std::shared_ptr<Node>& start);
	Array<Vec2> getRoute(const std::shared_ptr<Node>& start, const std::shared_ptr<Node>& end) const;

	const Array<Vec2>& getRoute() const { return m_route; }

	const MultiPolygon& getCuttingMultiPolygons() const { return m_cuttingMultiPolygons; }
	const MultiPolygon& getCirclingMultiPolygons() const { return m_circlingMultiPolygons; }
};
