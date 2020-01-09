#pragma once

#include "common.h"
#include "Command.h"

class RouteGenerator
{
	// temp
	MultiPolygon	m_cuttingMultiPolygons;
	MultiPolygon	m_circlingMultiPolygons;

	MultiPolygon	m_multiPolygon;
	Array<Vec2>		m_route;
	Vec2			m_workspaceSize;
	bool			m_isValid;

	double	m_cuttingMargin;
	double	m_cuttingInterval;
	double	m_circlingMargin;
	double	m_circlingInterval;

	void	updateRoute();
	void	updateValid();

	void	update();

public:
	RouteGenerator()
		: m_cuttingMargin(0.0)
		, m_circlingMargin(0.0)
		, m_cuttingInterval(0.5)
		, m_circlingInterval(m_circlingMargin)
		, m_workspaceSize(182 - 10, 128 - 10)
		, m_isValid(true)
	{}

	Array<Command>	getCommands() const;

	bool	isValid() const { return m_isValid; }

	void	setWorkspaceSize(const Vec2& workspaceSize) { m_workspaceSize = workspaceSize; update(); }
	void	setCuttingMargin(const double margin) { m_cuttingMargin = margin; update(); }
	void	setCirclingMargin(const double margin) { m_circlingMargin = margin; m_circlingInterval = m_circlingMargin; update(); }
	//void	setCuttingInterval(const double interval) { m_cuttingInterval = interval; update(); }
	//void	setCirclingInterval(const double interval) { m_circlingInterval = interval; update(); }
	void	addPolygon(const Polygon& polygon) { m_multiPolygon.emplace_back(polygon); update(); }

	const Array<Vec2>& getRoute() const { return m_route; }
};
