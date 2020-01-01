#pragma once

#include "common.h"
#include "Command.h"

class RouteGenerator
{
	MultiPolygon	m_multiPolygon;

public:
	Array<Command>	getCommands() const;
};
