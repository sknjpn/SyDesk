#include "MainViewer.h"
#include "GUISlider.h"

const int itemHeight = 40;

void MainViewer::SerialSelector::SerialList::init()
{
	setViewerRectInLocal(0, 32, 640, 400);

	addChildViewer<GUISlider>(0)
		->setViewerRectInLocal(600, 0, 40, 400);
}

void MainViewer::SerialSelector::SerialList::update()
{
	RectF(getViewerSize()).draw(Palette::White).drawFrame(2.0, 0.0, Palette::Black);

	// 追加
	{
		const auto serialPorts = System::EnumerateSerialPorts();
		const auto items = getChildViewers<Item>();

		for (const auto& serialPort : serialPorts)
		{
			if (!items.any([&serialPort](const auto& item) { return item->m_serialPort.port == serialPort.port; }))
			{
				addChildViewer<Item>(serialPort);
			}
		}
	}

	// 整列
	{
		const auto items = getChildViewers<Item>();

		getChildViewer<GUISlider>()->setHeight(items.size() * itemHeight);

		const auto delta = -getChildViewer<GUISlider>()->getDelta();

		// Layer
		for (int i = 0; i < items.size(); ++i)
		{
			const auto& item = getChildViewers<Item>()[i];

			item->setViewerPosInLocal(0, delta + itemHeight * i);
		}
	}
}
