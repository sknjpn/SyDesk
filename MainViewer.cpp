#include "MainViewer.h"

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

void MainViewer::update()
{
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

		if(image) addChildViewer<ImageDialog>(m_reservedItems.front(),image);
	
		m_reservedItems.pop_front();
	}

	try
	{
		m_communicator.update();
	}
	catch (...)
	{
		System::Exit();
	}
}

void MainViewer::setPolygon(const Polygon& polygon)
{
	//m_routeGenerator.addPolygon(Circle(25, 25, 25).asPolygon(500));
	m_communicator.addCommands(m_routeGenerator.getCommands());
}
