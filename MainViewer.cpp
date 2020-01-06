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
}

void MainViewer::update()
{
}
