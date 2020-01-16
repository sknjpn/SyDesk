#include "MainViewer.h"
#include "GUIText.h"

void MainViewer::SerialSelector::init()
{
	setViewerSize(640, 480);
	setViewerSize(0, 0);

	addChildViewer<GUIText>(U"Tiny Fabricaの接続されているPortを選択してください", Font(24, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
		->setViewerRectInLocal(10, 10, 620, 40);

	addChildViewer<GUIText>(U"もし、Tiny FabricaのPortが分からない場合、Tiny FabricaのUSBを挿し直してください。\nその時にオレンジになったPortがTiny FabricaのPortです！\nこの操作は間違っていてもやり直すことができます。", Font(12), GUIText::Mode::DrawInBox)
		->setViewerRectInLocal(10, 410, 620, 60);

	// List
	addChildViewer<SerialList>();

	setViewerPosInLocal(Scene::Center() - getViewerSize() / 2.0);
}

void MainViewer::SerialSelector::update()
{
	setViewerPosInLocal(Scene::Center() - getViewerSize() / 2.0);

	RectF(getViewerSize()).draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);
}

void MainViewer::SerialSelector::connect(const String& port)
{
	Communicator::Connect(port);

	getParentViewer<MainViewer>()->addChildViewer<OriginAdjustment>();

	destroy();
}
