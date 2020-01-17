#include "MainViewer.h"
#include "RouteGenerator.h"

void MainViewer::Workspace::init()
{

}

void MainViewer::Workspace::update()
{
	RectF(getViewerSize()).stretched(-5).draw(ColorF(0.9)).drawFrame(2.0, 0.0, Palette::Black);

	const auto workspaceSize = RouteGenerator::GetInstance()->getWorkspaceSize();
	auto s = Min((getViewerSize().x - 10.0) / workspaceSize.x, (getViewerSize().y - 10) / workspaceSize.y);
	auto t = Transformer2D(Mat3x2::Scale(s).translated(5, 5), true);

	RouteGenerator::GetInstance()->draw(s);
}
