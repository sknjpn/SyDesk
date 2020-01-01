#pragma once

#include "common.h"
#include "Command.h"

class RouteGenerator
{
	MultiPolygon	m_multiPolygon;

public:
	Array<Command>	getCommands() const;

	void	addPolygon(const Polygon& polygon) { m_multiPolygon.emplace_back(polygon); }
};
