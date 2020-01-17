#include "MainViewer.h"
#include "RouteGenerator.h"
#include "Communicator.h"
#include "GUIText.h"
#include "GUIButton.h"

void MainViewer::SerialViewer::init()
{
	addChildViewer<GUIButton>(U"カットスタート", [this]() { onStart(); }, false)
		->setName(U"カットスタート")
		->setViewerRectInLocal(10, 10, 230, 30);

	addChildViewer<GUIButton>(U"カット中断", [this]() { onInterruption(); }, false)
		->setName(U"カット中断")
		->setViewerRectInLocal(10, 50, 230, 30);

	// カット情報
	addChildViewer<GUIText>(U"", Font(16, Typeface::Bold), GUIText::Mode::DrawInBox)
		->setName(U"カット情報")
		->setViewerRectInLocal(10, 80, 230, 120);

	addChildViewer<GUIButton>(U"原点再調整", [this]() { onOriginAdjust(); }, false)
		->setName(U"原点再調整")
		->setViewerRectInLocal(10, 240, 230, 30);

	addChildViewer<GUIButton>(U"ポート再選択", [this]() { onReconnect(); }, false)
		->setName(U"ポート再選択")
		->setViewerRectInLocal(10, 290, 230, 30);
}

void MainViewer::SerialViewer::update()
{
	RectF(getViewerSize()).stretched(-5).draw(ColorF(0.9)).drawFrame(2.0, 0.0, Palette::Black);

	getChildViewer<GUIButton>(U"カットスタート")->setIsEnabled(canStart());
	getChildViewer<GUIButton>(U"カット中断")->setIsEnabled(canInterruption());

	if (getParentViewer()->hasChildViewer<SerialSelector>())
	{
		getChildViewer<GUIText>(U"カット情報")
			->m_text = U"Tiny FabricaをPCに接続して接続を押してしてください";
		getChildViewer<GUIText>(U"カット情報")
			->m_color = Palette::Red;
	}
	else if (getParentViewer()->hasChildViewer<OriginAdjustment>())
	{
		getChildViewer<GUIText>(U"カット情報")
			->m_text = U"原点調整を完了してください";
		getChildViewer<GUIText>(U"カット情報")
			->m_color = Palette::Red;
	}
	else if (RouteGenerator::GetInstance()->isInUpdate())
	{
		getChildViewer<GUIText>(U"カット情報")
			->m_text = U"ルート構築中...\nしばらくお待ちください...";
		getChildViewer<GUIText>(U"カット情報")
			->m_color = Palette::Orange;
	}
	else if (RouteGenerator::GetInstance()->isValid())
	{
		const double time = RouteGenerator::GetInstance()->getCuttingTime();
		int min = int(time / 60);
		int sec = int(time - min * 60);

		getChildViewer<GUIText>(U"カット情報")
			->m_text = Format(U"カット可能\nカット時間:", min, U"分", sec, U"秒");

		getChildViewer<GUIText>(U"カット情報")
			->m_color = Palette::Black;
	}
	else
	{
		auto reason = RouteGenerator::GetInstance()->getFailReason();

		getChildViewer<GUIText>(U"カット情報")
			->m_text = reason;
		getChildViewer<GUIText>(U"カット情報")
			->m_color = Palette::Red;
	}


	getChildViewer<GUIButton>(U"原点再調整")->setIsEnabled(canOriginAdjust());
	getChildViewer<GUIButton>(U"ポート再選択")->setIsEnabled(canReconnect());
}

void MainViewer::SerialViewer::onReconnect()
{
	// 原点調整の削除
	if (auto viewer = getParentViewer()->getChildViewer<OriginAdjustment>()) viewer->destroy();

	getParentViewer()->addChildViewer<SerialSelector>();
}

void MainViewer::SerialViewer::onOriginAdjust()
{
	getParentViewer()->addChildViewer<OriginAdjustment>();
}

void MainViewer::SerialViewer::onStart()
{
	Communicator::AddCommands(RouteGenerator::GetInstance()->getCommands());
	m_cuttingSW.start();
}

void MainViewer::SerialViewer::onInterruption()
{
	Communicator::ClearCommands();
	Communicator::AddCommandAtFront(Command('C', 0, 0, 0, 0));

	getParentViewer()->addChildViewer<OriginAdjustment>();
}

bool MainViewer::SerialViewer::canReconnect()
{
	return !getParentViewer()->hasChildViewer<SerialSelector>();
}

bool MainViewer::SerialViewer::canOriginAdjust()
{
	return Communicator::IsConnected() && !getParentViewer()->hasChildViewer<OriginAdjustment>();
}

bool MainViewer::SerialViewer::canStart()
{
	return Communicator::IsConnected() && Communicator::GetCommands().size() < 2 && RouteGenerator::GetInstance()->isValid();
}

bool MainViewer::SerialViewer::canInterruption()
{
	return Communicator::IsConnected() && Communicator::GetCommands().size() > 2;
}

