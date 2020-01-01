#include "RouteGenerator.h"

Array<Command> RouteGenerator::getCommands() const
{
	Array<Vec2>	m_route;

	for (const auto& polygon : m_multiPolygon)
		m_route.append(polygon.outer());

	Array<Command> commands;
	for (const auto& p : m_route)
	{
		commands.emplace_back('P', p.x * 10, 2000, p.y * 10, 2000);
	}

	return commands;
}
