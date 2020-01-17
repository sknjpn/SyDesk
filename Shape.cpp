#include "Shape.h"
#include "RouteGenerator.h"

std::mutex Shape::g_mutex;

void Shape::MoveBy(const Vec2& delta)
{
	std::lock_guard<std::mutex> lock(g_mutex);

	m_polygon.moveBy(delta);
	m_cuttingPolygon.moveBy(delta);
	m_circlingPolygon.moveBy(delta);

	if (m_isInUpdate) m_isNeedUpdate = true;
}

void Shape::draw(double s, bool isMouseOver, double cuttingMargin, double circlingMargin)
{
	const auto& routeGenerator = RouteGenerator::GetInstance();

	std::lock_guard<std::mutex> lock(g_mutex);

	const Color polygonColor = m_isInUpdate ? Palette::Darkgreen : Palette::Green;
	const Color circlingColor = m_isInUpdate ? Palette::Darkblue : Palette::Blue;
	const Color cuttingColor = m_isInUpdate ? Palette::Darkorange : Palette::Orange;

	// polygon
	m_polygon.draw(ColorF(polygonColor, 0.8)).drawFrame(1.0, polygonColor);
	
	m_polygon.boundingRect().stretched(cuttingMargin + circlingMargin)
		.draw(ColorF(circlingColor, 0.1))
		.drawFrame(1.0 / s, Palette::Blue);

	if (isMouseOver)
		m_polygon.boundingRect().stretched(cuttingMargin + circlingMargin).draw(ColorF(1.0, 0.2));

	// circling
	if (!m_circlingPolygon.isEmpty())
	{
		m_circlingPolygon
			.draw(ColorF(circlingColor, 0.25))
			.drawFrame(2.0 / s, circlingColor);
	}

	// cutting
	if (!m_cuttingPolygon.isEmpty())
		m_cuttingPolygon.drawFrame(2.0 / s, ColorF(cuttingColor, 0.25));
}

void Shape::update()
{
	const auto& routeGenerator = RouteGenerator::GetInstance();

	// 状態の定義
	{
		std::lock_guard<std::mutex> lock(g_mutex);

		m_isInUpdate = true;
		m_isNeedUpdate = false;
	}

	// 一時データ
	double circlingMarginTemp;
	double cuttingMarginTemp;
	Polygon	polygonTemp;
	Polygon	cuttingPolygonTemp;
	Polygon circlingPolygonTemp;

	// データコピー
	{
		cuttingMarginTemp = routeGenerator->getCuttingMargin();
		circlingMarginTemp = routeGenerator->getCirclingMargin();
		polygonTemp = getPolygon();
	}

	// バッファ計算
	{
		cuttingPolygonTemp = polygonTemp.calculateRoundBuffer(cuttingMarginTemp);
		circlingPolygonTemp = cuttingPolygonTemp.calculateRoundBuffer(circlingMarginTemp);
	}

	// 処理中断
	{
		std::lock_guard<std::mutex> lock(g_mutex);
		if (m_isNeedUpdate) { m_isInUpdate = false; return; }
	}

	// 最小距離の設定
	{
		Array<Vec2> outer;
		outer.emplace_back(circlingPolygonTemp.outer().front());

		for (auto& p : circlingPolygonTemp.outer())
			if (outer.back().distanceFrom(p) > circlingMarginTemp)
				outer.emplace_back(p);

		circlingPolygonTemp = Polygon(outer);
	}

	// データコピー
	{
		std::lock_guard<std::mutex> lock(g_mutex);
		if (m_isNeedUpdate) { m_isInUpdate = false; return; }

		m_cuttingPolygon = cuttingPolygonTemp;
		m_circlingPolygon = circlingPolygonTemp;
	}

	m_isInUpdate = false;
}
