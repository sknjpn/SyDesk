#include "MainViewer.h"
#include "GUIButton.h"

void MainViewer::SerialSelector::SerialList::Item::init()
{
	setViewerSize(640, 32);

	addChildViewer<GUIButton>(U"接続", [this]() {getParentViewer()->getParentViewer<SerialSelector>()->connect(m_serialPort.port); }, true)
		->setViewerRectInLocal(600, 4, 76, 28);
}

void MainViewer::SerialSelector::SerialList::Item::update()
{
	// 接続可能かどうか
	const bool canConnect = System::EnumerateSerialPorts().count(m_serialPort) == 1;

	getChildViewer<GUIButton>()->setIsEnabled(canConnect);
	if (!canConnect) m_isLatched = true;

	// 再接続が行われたらオレンジ色にする
	RectF(getViewerSize()).stretched(-4).draw((canConnect && m_isLatched) ? Palette::Orange : Palette::Gray);

	// Port Desc
	{
		static Font font(16);

		font(m_serialPort.port, U"--", m_serialPort.hardwareID, U"--", m_serialPort.description).draw();
	}
}
