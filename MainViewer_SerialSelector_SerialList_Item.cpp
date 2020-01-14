#include "MainViewer.h"
#include "GUIButton.h"
#include "GUIText.h"

void MainViewer::SerialSelector::SerialList::Item::init()
{
	setViewerSize(580, 32);

	addChildViewer<GUIButton>(U"接続", [this]() { getParentViewer()->getParentViewer<SerialSelector>()->connect(m_serialPort.port); }, true)
		->setViewerRectInLocal(500 + 2, 0 + 2, 80 - 4, 32 - 4);

	addChildViewer<GUIText>(m_serialPort.port, Font(24, Typeface::Black), GUIText::Mode::DrawLeftCenter)
		->setViewerRectInLocal(10, 0, 160, 32);

	addChildViewer<GUIText>(U"", Font(20), GUIText::Mode::DrawLeftCenter)
		->setName(U"State")
		->setViewerRectInLocal(180, 0, 300, 32);
}

void MainViewer::SerialSelector::SerialList::Item::update()
{
	RectF(getViewerSize()).draw(ColorF(0.7)).drawFrame(2.0, 0.0, Palette::Black);

	// set State
	{
		const bool canConnect = System::EnumerateSerialPorts().any([this](const auto& serialPortInfo) { return m_serialPort.port == serialPortInfo.port; });

		getChildViewer<GUIButton>()->setIsEnabled(canConnect);
		if (!canConnect) m_state = State::DisConnected;
		else if (m_state == State::DisConnected) m_state = State::ReConnected;
	}

	// draw Rect
	{
		Color color = ColorF(0.6);
		switch (m_state)
		{
		case MainViewer::SerialSelector::SerialList::Item::State::ReConnected: color = color = Palette::Orange; break;
		case MainViewer::SerialSelector::SerialList::Item::State::DisConnected: color = color = Palette::Gray; break;
		case MainViewer::SerialSelector::SerialList::Item::State::LastConnected: color = color = Palette::Blue; break;
		}

		RectF(getViewerSize()).stretched(-2)
			.draw(color)
			.drawFrame(1.0, 0.0, Palette::Black);
	}

	// set State Text
	switch (m_state)
	{
	case MainViewer::SerialSelector::SerialList::Item::State::None: getChildViewer<GUIText>(U"State")->m_text = U""; break;
	case MainViewer::SerialSelector::SerialList::Item::State::ReConnected: getChildViewer<GUIText>(U"State")->m_text = U"新規接続したPortです"; break;
	case MainViewer::SerialSelector::SerialList::Item::State::DisConnected: getChildViewer<GUIText>(U"State")->m_text = U"切断されたPortです"; break;
	case MainViewer::SerialSelector::SerialList::Item::State::LastConnected: getChildViewer<GUIText>(U"State")->m_text = U"前回接続したPortです"; break;
	}
}
