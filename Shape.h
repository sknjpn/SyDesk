#pragma once

#include "common.h"
#include <mutex>

#include "RouteGenerator.h"

extern std::mutex g_shapeMutex;

class Shape
{
	Polygon	m_cuttingPolygonTemp;
	Polygon m_circlingPolygonTemp;

public:
	bool m_isInUpdate = false;
	bool m_isNeedUpdate = true;

	bool m_isGrabbed = false;
	Polygon	m_polygon;			// read only
	Polygon	m_cuttingPolygon;	// read or write
	Polygon m_circlingPolygon;	// read or write

public:
	Shape()
	{

	}

	void update(const RouteGenerator& routeGenerator);
};
