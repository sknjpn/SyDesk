#include "MainViewer.h"
#include "GUISlider.h"
#include "GUIText.h"
#include "GUIButton.h"
#include "GUIValuer.h"

void MainViewer::CutSetting::init()
{
	INIData ini(U"config.ini");

	// Cutting Marginの設定
	addChildViewer<GUIText>(U"切り白", Font(24, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
		->setName(U"CuttingMarginTitle")
		->setViewerRectInLocal(10, 10, 230, 30);

	addChildViewer<GUIValuer>(ini.get<double>(U"CutSetting", U"CuttingMargin"))
		->setName(U"CuttingMarginValuer")
		->setViewerRectInLocal(10, 50, 230, 30);


	// Circling Marginの設定
	addChildViewer<GUIText>(U"安全幅", Font(24, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
		->setName(U"CirclingMarginTitle")
		->setViewerRectInLocal(10, 110, 230, 30);
	addChildViewer<GUIValuer>(ini.get<double>(U"CutSetting", U"CirclingMargin"))
		->setName(U"CirclingMarginValuer")
		->setViewerRectInLocal(10, 150, 230, 30);

	// カット速度の設定 2000 = 5mm/s   max 8mm/s
	addChildViewer<GUIText>(U"カット速度", Font(24, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
		->setName(U"CuttingSpeedTitle")
		->setViewerRectInLocal(10, 210, 230, 30);
	addChildViewer<GUIValuer>(ini.get<double>(U"CutSetting", U"CuttingSpeed"))
		->setName(U"CuttingSpeedValuer")
		->setViewerRectInLocal(10, 250, 230, 30);

	// カット速度の設定
	addChildViewer<GUIText>(U"電熱線出力", Font(24, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
		->setName(U"WireTemperatureTitle")
		->setViewerRectInLocal(10, 310, 230, 30);
	addChildViewer<GUIValuer>(ini.get<double>(U"CutSetting", U"WireTemperature"))
		->setName(U"WireTemperatureValuer")
		->setViewerRectInLocal(10, 350, 230, 30);
}

void MainViewer::CutSetting::update()
{
	RectF(getViewerSize()).stretched(-5).draw(ColorF(0.9)).drawFrame(2.0, 0.0, Palette::Black);

	auto& routeGenerator = getParentViewer<MainViewer>()->m_routeGenerator;

	INIData ini(U"config.ini");

	// 値の適用
	const auto cuttingMargin = getChildViewer<GUIValuer>(U"CuttingMarginValuer")->m_value * 5.0;
	getChildViewer<GUIText>(U"CuttingMarginTitle")->m_text = Format(U"切り白:", cuttingMargin, U"mm");
	ini.write<double>(U"CutSetting", U"CuttingMargin", getChildViewer<GUIValuer>(U"CuttingMarginValuer")->m_value);

	const auto circlingMargin = Math::Lerp(1.0, 10.0, getChildViewer<GUIValuer>(U"CirclingMarginValuer")->m_value);
	getChildViewer<GUIText>(U"CirclingMarginTitle")->m_text = Format(U"安全幅:", circlingMargin, U"mm");
	ini.write<double>(U"CutSetting", U"CirclingMargin", getChildViewer<GUIValuer>(U"CirclingMarginValuer")->m_value);

	const auto cuttingSpeed = 10000.0 / Math::Lerp(0.5, 8.0, getChildViewer<GUIValuer>(U"CuttingSpeedValuer")->m_value);
	getChildViewer<GUIText>(U"CuttingSpeedTitle")->m_text = Format(U"カット速度:", cuttingSpeed, U"mm/s");
	ini.write<double>(U"CutSetting", U"CuttingSpeed", getChildViewer<GUIValuer>(U"CuttingSpeedValuer")->m_value);

	const auto wireTemperature = getChildViewer<GUIValuer>(U"WireTemperatureValuer")->m_value * 100.0;
	getChildViewer<GUIText>(U"WireTemperatureTitle")->m_text = Format(U"電熱線出力:", wireTemperature, U"%");
	ini.write<double>(U"CutSetting", U"WireTemperature", getChildViewer<GUIValuer>(U"WireTemperatureValuer")->m_value);

	if (routeGenerator.m_cuttingMargin != cuttingMargin || routeGenerator.m_circlingMargin != circlingMargin)
		getParentViewer<MainViewer>()->getChildViewer<Workspace>()->onMarginChanged();

	ini.save(U"config.ini");

	routeGenerator.m_cuttingMargin = cuttingMargin;
	routeGenerator.m_circlingMargin = circlingMargin;
	routeGenerator.m_cuttingSpeed = cuttingSpeed;
	routeGenerator.m_wireTemperature = wireTemperature;
}
