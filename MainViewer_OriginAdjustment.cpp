#include "MainViewer.h"
#include "GUIText.h"
#include "GUIButton.h"

void MainViewer::OriginAdjustment::init()
{
	setViewerSize(640, 480);

	addChildViewer<GUIButton>(U"完了", [this]() { destroy(); })
		->setViewerRectInLocal(20, 240, 200, 40);

	addChildViewer<GUIText>(U"Tiny Fabricaのワークテーブル(発泡スチロールを載せる台)を原点(カットを開始する点)にボタンをおして移動してください。\n全体をカットする場合、電熱線がワークテーブルの右上にあたるようにしてください\n左側のボタンを押すか、キーボードの矢印キーを押すことで動かすことができます。\n移動出来たら、「完了」ボタンをクリックするかEnterキーを押してください！\nこの操作は後からやり直すことが出来ます。", Font(12), GUIText::Mode::DrawInBox)
		->setViewerRectInLocal(240 + 10, 200 + 20, 400 - 20, 160);

	setViewerPosInLocal(Scene::Center() - getViewerSize() / 2.0);
}

void MainViewer::OriginAdjustment::update()
{
	setViewerPosInLocal(Scene::Center() - getViewerSize() / 2.0);

	// シリアルが切断されている場合は削除
	if (!Communicator::IsConnected())
	{
		destroy();

		return;
	}

	RectF(getViewerSize()).draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);

	// 表示
	/*
	原点が電熱線の位置に来るようにワークテーブルを移動させてください。
	移動には、上下左右のボタンを押すか、キーボードの上下左右キーを押してください。

	*/

	// UIパネル
	{
		// 上下左右移動
		{
			const int length = 60;
			Rect(0, 0, length * 4, length * 5).stretched(-5).rounded(10).draw(ColorF(0.9)).drawFrame(2.0, 0.0, Palette::Black);

			const Circle baseCircle(40);
			const auto circleU = baseCircle.movedBy(length * 2, length * 1);
			const auto circleR = baseCircle.movedBy(length * 3, length * 2);
			const auto circleD = baseCircle.movedBy(length * 2, length * 3);
			const auto circleL = baseCircle.movedBy(length * 1, length * 2);
			circleU.draw(Palette::Lightblue);
			circleR.draw(Palette::Lightblue);
			circleD.draw(Palette::Lightblue);
			circleL.draw(Palette::Lightblue);
			m_textureU.drawAt(circleU.center, m_stateNowUD == StateUD::MoveToU ? Palette::Orange : circleU.mouseOver() ? Palette::Gray : Palette::Black);
			m_textureR.drawAt(circleR.center, m_stateNowRL == StateRL::MoveToR ? Palette::Orange : circleR.mouseOver() ? Palette::Gray : Palette::Black);
			m_textureD.drawAt(circleD.center, m_stateNowUD == StateUD::MoveToD ? Palette::Orange : circleD.mouseOver() ? Palette::Gray : Palette::Black);
			m_textureL.drawAt(circleL.center, m_stateNowRL == StateRL::MoveToL ? Palette::Orange : circleL.mouseOver() ? Palette::Gray : Palette::Black);

			m_stateNowRL = StateRL::None;
			m_stateNowUD = StateUD::None;
			if (circleU.leftPressed()) m_stateNowUD = StateUD::MoveToU;
			if (circleR.leftPressed()) m_stateNowRL = StateRL::MoveToR;
			if (circleD.leftPressed()) m_stateNowUD = StateUD::MoveToD;
			if (circleL.leftPressed()) m_stateNowRL = StateRL::MoveToL;
		}

		// 接続遮断
		{

		}

		// 説明
		{
			m_explain.draw(Vec2(250, 20));
		}
	}

	// キー入力による左右
	{
		if (KeyRight.pressed()) m_stateNowRL = StateRL::MoveToR;
		else if (KeyLeft.pressed()) m_stateNowRL = StateRL::MoveToL;

		if (KeyUp.pressed()) m_stateNowUD = StateUD::MoveToU;
		else if (KeyDown.pressed()) m_stateNowUD = StateUD::MoveToD;
	}

	// Serialによる変更適用
	if (m_stateNowRL != m_statePreRL || m_stateNowUD != m_statePreUD)
	{
		short param = 0b10000;
		if (m_stateNowRL == StateRL::MoveToL) param += 0b0100;
		if (m_stateNowUD == StateUD::MoveToU) param += 0b0010;
		if (m_stateNowRL == StateRL::MoveToR) param += 0b0001;
		if (m_stateNowUD == StateUD::MoveToD) param += 0b1000;
		Communicator::AddCommand(Command('K', param, short(getParentViewer<MainViewer>()->getChildViewer<CutSetting>()->m_cuttingSpeed), 0, 0));

		m_statePreRL = m_stateNowRL;
		m_statePreUD = m_stateNowUD;
	}

	if (KeyEnter.down()) destroy();
}
