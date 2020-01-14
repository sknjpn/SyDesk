#include "MainViewer.h"

void MainViewer::Adviser::init()
{
}

void MainViewer::Adviser::update()
{
	RectF(getViewerSize()).draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);
}
