#pragma once

#include "common.h"
#include <mutex>

class Shape
{
	static std::mutex g_mutex;

	bool m_isInUpdate = false;
	bool m_isNeedUpdate = true;

	Array<Vec2> outer;
	Polygon	m_polygon;
	Polygon	m_cuttingPolygon;
	Polygon m_circlingPolygon;

public:
	bool m_isGrabbed = false;	// used by only GUI

public:
	Shape(Polygon polygon)
		: m_polygon(polygon)
	{

	}

	// Move
	void MoveBy(const Vec2& delta);

	// Get (衝突の危険回避のために参照は使わない)
	Polygon getPolygon() const { std::lock_guard<std::mutex> lock(g_mutex); return m_polygon; }
	Polygon getCuttingPolygon() const { std::lock_guard<std::mutex> lock(g_mutex); return m_cuttingPolygon; }
	Polygon getCirclingPolygon() const { std::lock_guard<std::mutex> lock(g_mutex); return m_circlingPolygon; }

	// Flag
	bool isInUpdate() const { std::lock_guard<std::mutex> lock(g_mutex); return m_isInUpdate; }
	bool isNeedUpdate() const { std::lock_guard<std::mutex> lock(g_mutex); return m_isNeedUpdate; }
	void setNeedUpdate() { std::lock_guard<std::mutex> lock(g_mutex); m_isNeedUpdate = true; }

	void draw(double s, bool isMouseOver, double cuttingMargin, double circlingMargin);

	// Update
	void update();
};
