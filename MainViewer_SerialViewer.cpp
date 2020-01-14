#include "MainViewer.h"
#include "GUIText.h"
#include "GUIButton.h"

void MainViewer::SerialViewer::init()
{
}

void MainViewer::SerialViewer::update()
{
	RectF(getViewerSize()).stretched(-5).draw(ColorF(0.9)).drawFrame(2.0, 0.0, Palette::Black);

	// 通信状態(authorized)の表示

	// カット開始ボタン

	// 
}
