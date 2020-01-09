﻿#include "MainViewer.h"
#include "GUIText.h"

void MainViewer::SerialSelector::init()
{
	setViewerSize(640, 480);
	setViewerPosInLocal(Scene::Center() - getViewerSize() / 2.0);

	addChildViewer<GUIText>(U"Tiny Fabricaの接続されているPortを選択してください", Font(24, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
		->setViewerRectInLocal(10, 10, 620, 40);

	addChildViewer<GUIText>(U"もし、Tiny FabricaのPortが分からない場合、Tiny FabricaのUSBを挿し直してください。\nその時にオレンジになったPortがTiny FabricaのPortです！\nこの操作は間違っていてもやり直すことができます。", Font(12), GUIText::Mode::DrawInBox)
		->setViewerRectInLocal(10, 410, 620, 60);

	// List
	addChildViewer<SerialList>();
}

void MainViewer::SerialSelector::update()
{
	RectF(getViewerSize()).draw(Palette::White).drawFrame(2.0, 0.0, Palette::Black);
}

void MainViewer::SerialSelector::connect(const String& port)
{
	getParentViewer<MainViewer>()->m_communicator.connect(port);

	getParentViewer<MainViewer>()->addChildViewer<OriginAdjustment>();

	destroy();
}
