#include "MainViewer.h"

void MainViewer::SerialSelector::init()
{
	setViewerSize(640, 480);
	setViewerPosInLocal(Scene::Center() - getViewerSize() / 2.0);
}

void MainViewer::SerialSelector::update()
{
	// Serial List
	{
		static Font font(24);
		const auto serialPorts = System::EnumerateSerialPorts();

		for (const auto& serialPort : serialPorts)
		{
			font(serialPort.port, U"--", serialPort.hardwareID, U"--", serialPort.description).draw();

			moveDrawPos(0, 32);
		}
	}
}
