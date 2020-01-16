#include "MainViewer.h"
#include "GUISlider.h"
#include "GUIText.h"
#include "GUIButton.h"
#include "GUIValuer.h"

void MainViewer::CutSetting::onCuttingMarginChanged(double value)
{
	INIData ini(U"config.ini");

	m_cuttingMargin = value * 5.0;

	getParentViewer<MainViewer>()->m_routeGenerator.setCuttingMargin(m_cuttingMargin);
	getParentViewer<MainViewer>()->getChildViewer<Workspace>()->onMarginChanged();

	ini.write<double>(U"CutSetting", U"CuttingMargin", value);
	ini.save(U"config.ini");
}

void MainViewer::CutSetting::onCirclingMarginChanged(double value)
{
	INIData ini(U"config.ini");

	m_circlingMargin = Math::Lerp(1.0, 10.0, value);

	getParentViewer<MainViewer>()->m_routeGenerator.setCirclingMargin(m_circlingMargin);
	getParentViewer<MainViewer>()->getChildViewer<Workspace>()->onMarginChanged();

	ini.write<double>(U"CutSetting", U"CirclingMargin", value);
	ini.save(U"config.ini");
}

void MainViewer::CutSetting::onCuttingSpeedChanged(double value)
{
	INIData ini(U"config.ini");

	m_cuttingSpeed = 10000.0 / Math::Lerp(0.5, 8.0, value);

	getParentViewer<MainViewer>()->m_routeGenerator.setCuttingSpeed(m_cuttingSpeed);

	ini.write<double>(U"CutSetting", U"CuttingSpeed", value);
	ini.save(U"config.ini");
}

void MainViewer::CutSetting::onWireTemperatureChanged(double value)
{
	INIData ini(U"config.ini");

	m_wireTemperature = value * 255.0;

	ini.write<double>(U"CutSetting", U"WireTemperature", value);
	ini.save(U"config.ini");

	if (getParentViewer<MainViewer>()->m_communicator.getSerial().isOpened())
		getParentViewer<MainViewer>()->m_communicator.addCommandAtFront(Command('H', short(m_wireTemperature), short(0), short(0), short(0)));
}

void MainViewer::CutSetting::init()
{
	const INIData ini(U"config.ini");
	double cuttingMarginValue = ini.get<double>(U"CutSetting", U"CuttingMargin");
	double circlingMarginValue = ini.get<double>(U"CutSetting", U"CirclingMargin");
	double cuttingSpeedValue = ini.get<double>(U"CutSetting", U"CuttingSpeed");
	double WireTemperatureValue = ini.get<double>(U"CutSetting", U"WireTemperature");

	// Cutting Marginの設定
	addChildViewer<GUIText>(U"切り白", Font(24, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
		->setName(U"CuttingMarginTitle")
		->setViewerRectInLocal(10, 10, 230, 30);

	addChildViewer<GUIValuer>([this](double value) { onCuttingMarginChanged(value); }, cuttingMarginValue)
		->setViewerRectInLocal(10, 50, 230, 30);


	// Circling Marginの設定
	addChildViewer<GUIText>(U"安全幅", Font(24, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
		->setName(U"CirclingMarginTitle")
		->setViewerRectInLocal(10, 110, 230, 30);

	addChildViewer<GUIValuer>([this](double value) { onCirclingMarginChanged(value); }, circlingMarginValue)
		->setViewerRectInLocal(10, 150, 230, 30);


	// カット速度の設定 2000 = 5mm/s   max 8mm/s
	addChildViewer<GUIText>(U"カット速度", Font(24, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
		->setName(U"CuttingSpeedTitle")
		->setViewerRectInLocal(10, 210, 230, 30);
	
	addChildViewer<GUIValuer>([this](double value) { onCuttingSpeedChanged(value); }, cuttingSpeedValue)
		->setViewerRectInLocal(10, 250, 230, 30);


	// カット温度の設定
	addChildViewer<GUIText>(U"電熱線出力", Font(24, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
		->setName(U"WireTemperatureTitle")
		->setViewerRectInLocal(10, 310, 230, 30);
	
	addChildViewer<GUIValuer>([this](double value) { onWireTemperatureChanged(value); }, WireTemperatureValue)
		->setViewerRectInLocal(10, 350, 230, 30);

	onCuttingMarginChanged(cuttingMarginValue);
	onCirclingMarginChanged(circlingMarginValue);
	onCuttingSpeedChanged(cuttingSpeedValue);
	onWireTemperatureChanged(WireTemperatureValue);
}

void MainViewer::CutSetting::update()
{
	RectF(getViewerSize()).stretched(-5).draw(ColorF(0.9)).drawFrame(2.0, 0.0, Palette::Black);

	// 値の適用
	getChildViewer<GUIText>(U"CuttingMarginTitle")->m_text = Format(U"切り白:", m_cuttingMargin, U"mm");
	getChildViewer<GUIText>(U"CirclingMarginTitle")->m_text = Format(U"安全幅:", m_circlingMargin, U"mm");
	getChildViewer<GUIText>(U"CuttingSpeedTitle")->m_text = Format(U"カット速度:", m_cuttingSpeed, U"mm/s");
	getChildViewer<GUIText>(U"WireTemperatureTitle")->m_text = Format(U"電熱線出力:", short(m_wireTemperature), U"%");
}
