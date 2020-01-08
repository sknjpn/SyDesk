#include "MainViewer.h"

void MainViewer::OriginAdjustment::init()
{
}

void MainViewer::OriginAdjustment::update()
{
	const auto& communicator = getParentViewer<MainViewer>()->m_communicator;

	// 表示
	/*
	原点が電熱線の位置に来るようにワークテーブルを移動させてください。
	移動には、上下左右のボタンを押すか、キーボードの上下左右キーを押してください。
	
	*/

	// 接続が切れた場合の処理
	if (!communicator.getSerial().isOpened())
	{
		Logger << U"接続が遮断されました";

		// 再接続
		{
			destroy();

			getParentViewer()->addChildViewer<SerialSelector>();

			return;
		}
	}

	// UIパネル
	{
		// 上下左右移動
		{

		}

		// 原点調整完了
		{
			destroy();

			return;
		}
		
		// 接続遮断
		{

		}
	}

	// キー入力による左右
	{
		if (KeyRight.pressed()) m_stateNowRL = StateRL::MoveToRight;
		else if (KeyLeft.pressed()) m_stateNowRL = StateRL::MoveToRight;
		else m_stateNowRL = StateRL::None;

		if (KeyUp.pressed()) m_stateNowTD = StateTD::MoveToTop;
		else if (KeyDown.pressed()) m_stateNowTD = StateTD::MoveToDown;
		else m_stateNowTD = StateTD::None;
	}

	// Serialによる変更適用
	if (m_stateNowRL != m_statePreRL || m_stateNowTD != m_statePreTD)
	{
		m_statePreRL = m_stateNowRL;
		m_statePreTD = m_stateNowTD;
	}
}
