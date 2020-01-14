#include "MainViewer.h"

void MainViewer::MachineViewer::init()
{
}

void MainViewer::MachineViewer::update()
{
	RectF(getViewerSize()).draw(ColorF(0.9)).drawFrame(2.0, 0.0, Palette::Black);
}
