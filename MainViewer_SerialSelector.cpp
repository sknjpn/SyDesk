#include "MainViewer.h"

void MainViewer::SerialSelector::init()
{
	setViewerSize(640, 480);
	setViewerPosInLocal(Scene::Center() - getViewerSize() / 2.0);

	// List
	addChildViewer<SerialList>();
}

void MainViewer::SerialSelector::update()
{
	RectF(getViewerSize()).draw(Palette::White).drawFrame(2.0, 0.0, Palette::Black);

	moveDrawPos(5, 5);

	// Title
	{
		static Font font(24);

		font(U"Tiny Fabricaの接続されているPortを選択してください").draw();

		moveDrawPos(0, 32);
	}

	moveDrawPos(0, 400);

	// Description
	{
		static Font font(16);

		font(U"もし、Tiny FabricaのPortが分からない場合、Tiny FabricaのUSBを挿し直してください。\nその時に変化したPortがTiny FabricaのPortです！\nこの操作は間違っていてもやり直すことができます。").draw();
	}
}

void MainViewer::SerialSelector::connect(const String& port)
{
	destroy();
}
