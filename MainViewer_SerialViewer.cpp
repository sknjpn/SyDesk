#include "MainViewer.h"
#include "GUIText.h"
#include "GUIButton.h"

void MainViewer::SerialViewer::init()
{
	addChildViewer<GUIButton>(U"カットスタート", [this]() { onStart(); }, false)
		->setName(U"カットスタート")
		->setViewerRectInLocal(10, 10, 230, 30);

	addChildViewer<GUIButton>(U"カット中断", [this]() { onInterruption(); }, false)
		->setName(U"カット中断")
		->setViewerRectInLocal(10, 60, 230, 30);

	addChildViewer<GUIButton>(U"原点再調整", [this]() { onOriginAdjust(); }, false)
		->setName(U"原点再調整")
		->setViewerRectInLocal(10, 110, 230, 30);

	addChildViewer<GUIButton>(U"ポート再選択", [this]() { onReconnect(); }, false)
		->setName(U"ポート再選択")
		->setViewerRectInLocal(10, 160, 230, 30);
}

void MainViewer::SerialViewer::update()
{
	RectF(getViewerSize()).stretched(-5).draw(ColorF(0.9)).drawFrame(2.0, 0.0, Palette::Black);

	getChildViewer<GUIButton>(U"カットスタート")->setIsEnabled(canStart());
	getChildViewer<GUIButton>(U"カット中断")->setIsEnabled(canInterruption());
	getChildViewer<GUIButton>(U"原点再調整")->setIsEnabled(canOriginAdjust());
	getChildViewer<GUIButton>(U"ポート再選択")->setIsEnabled(canReconnect());
}

void MainViewer::SerialViewer::onReconnect()
{
	// 原点調整の削除
	if (auto viewer = getParentViewer()->getChildViewer<OriginAdjustment>()) viewer->destroy();

	getParentViewer()->addChildViewer<SerialSelector>();
}

void MainViewer::SerialViewer::onOriginAdjust()
{
	getParentViewer()->addChildViewer<OriginAdjustment>();
}

void MainViewer::SerialViewer::onStart()
{
	getParentViewer<MainViewer>()->m_communicator.addCommands(getParentViewer<MainViewer>()->m_routeGenerator.getCommands());
}

void MainViewer::SerialViewer::onInterruption()
{
	getParentViewer<MainViewer>()->m_communicator.clearCommands();
	getParentViewer<MainViewer>()->m_communicator.addCommandAtFront(Command('C', 0, 0, 0, 0));
}

bool MainViewer::SerialViewer::canReconnect()
{
	return !getParentViewer()->hasChildViewer<SerialSelector>();
}

bool MainViewer::SerialViewer::canOriginAdjust()
{
	return getParentViewer<MainViewer>()->m_communicator.isConnected() && !getParentViewer()->hasChildViewer<OriginAdjustment>();
}

bool MainViewer::SerialViewer::canStart()
{
	return getParentViewer<MainViewer>()->m_communicator.isConnected() && getParentViewer<MainViewer>()->m_routeGenerator.isValid();
}

bool MainViewer::SerialViewer::canInterruption()
{
	return getParentViewer<MainViewer>()->m_communicator.isConnected() && getParentViewer<MainViewer>()->m_communicator.getCommands().size() > 5;
}

