#include "MainViewer.h"
#include "RouteGenerator.h"
#include "Communicator.h"

// ワークフロー
// 1, SerialSelector
// 2, OriginAdjustment
// 3, Work

void MainViewer::init()
{
	addChildViewer<Workspace>();
	addChildViewer<CutSetting>();
	addChildViewer<SerialViewer>();

	// 最初のみ
	addChildViewer<SerialSelector>();

	setBackgroundColor(Palette::Gray);
}

Stopwatch heatSW(true);

void MainViewer::update()
{
	if (heatSW.s() > 1)
	{
		heatSW.restart();

		if (Communicator::GetSerial().isOpened())
			Communicator::AddCommandAtFront(Command('H', short(getChildViewer<CutSetting>()->m_wireTemperature), short(0), short(0), short(0)));
	}

	setViewerSize(Scene::Size());

	// UIの配置調整

	getChildViewer<Workspace>()
		->setViewerRectInLocal(250, 0, Scene::Size().x - 500, Scene::Size().y);

	getChildViewer<CutSetting>()
		->setViewerRectInLocal(Scene::Size().x - 250, 0, 250, Scene::Size().y);

	getChildViewer<SerialViewer>()
		->setViewerRectInLocal(0, 0, 250, Scene::Size().y);

	if (DragDrop::HasNewFilePaths())
	{
		const auto items = DragDrop::GetDroppedFilePaths();

		m_reservedItems.append(items);
	}

	if (!m_reservedItems.isEmpty() && !hasChildViewer<ImageDialog>())
	{
		const Image image(m_reservedItems.front().path);

		if (image) addChildViewer<ImageDialog>(m_reservedItems.front(), image);

		m_reservedItems.pop_front();
	}

	RouteGenerator::GetInstance()->update();
	Communicator::Update();

	if (!Communicator::IsConnected() && !hasChildViewer<SerialSelector>())
		addChildViewer<SerialSelector>();
}
