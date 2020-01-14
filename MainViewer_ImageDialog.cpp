#include "MainViewer.h"
#include "GUIButton.h"
#include "GUIText.h"
#include "GUITextBox.h"

Array<Vec2> MainViewer::ImageDialog::getOutline(const Image& image, std::function<bool(Color)> judge)
{
	const Point directions[8] = { { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 } };
	const Rect rect(image.size());
	Grid<bool> map(rect.w, rect.h, false);
	Point startPosition;

	// mapに転写
	Point p = Point::Zero();
	for (;;)
	{
		if (!map[p] && judge(image[p]))
		{
			Array<Point> points;

			// 始点
			startPosition = p;
			map[p] = true;
			points.emplace_back(p);

			for (int i = 0; i < points.size(); ++i)
			{
				for (int dir = 0; dir < 8; ++dir)
				{
					const Point point = points[i].movedBy(directions[dir]);

					if (rect.contains(point) && !map[point] && judge(image[point])) { map[point] = true; points.emplace_back(point); }
				}
			}

			break;
		}

		// 読み取り位置変更
		if (++p.x == rect.w)
		{
			p.x = 0;

			// 結果無し
			if (++p.y == rect.h) return Array<Vec2>();
		}
	}

	int		botDirection = 0;
	Point	botPosition = startPosition;

	Array<Point> outline;
	outline.emplace_back(botPosition);

	for (;;)
	{
		for (int i = -2; i <= 4; ++i)
		{
			// 始点に戻った場合
			if (outline.size() >= 2 && (botDirection + i) % 8 == 0 && botPosition == outline.front())
			{
				// 最初と最後を被らせないために末尾を削除する
				outline.pop_back();

				// 転写
				Array<Vec2> result;
				for (const auto& o : outline)
					result.emplace_back(o.x, o.y);

				return result;
			}

			// 移動
			const Point forwardPosition = botPosition + directions[(botDirection + i + 8) % 8];
			if (rect.contains(forwardPosition) && map[forwardPosition.y][forwardPosition.x])
			{
				botPosition = forwardPosition;
				botDirection = ((botDirection + i) % 8 + 8) % 8;
				outline.emplace_back(botPosition);

				break;
			}

			// 一ブロックの場合は無視
			if (i == 4) return Array<Vec2>();
		}
	}
}

Array<Array<Vec2>> MainViewer::ImageDialog::getOutlines(const Image& image, std::function<bool(Color)> judge)
{
	const Point directions[8] = { { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 } };
	const Rect rect(image.size());
	Grid<bool> map(rect.w, rect.h, false);
	Array<Point> startPositions;

	// mapに転写
	{
		Point p = Point::Zero();
		for (;;)
		{
			if (!map[p] && judge(image[p]))
			{
				Array<Point> points;

				// 始点
				startPositions.emplace_back(p);
				map[p] = true;
				points.emplace_back(p);

				for (int i = 0; i < points.size(); ++i)
				{
					for (int dir = 0; dir < 8; ++dir)
					{
						const Point point = points[i].movedBy(directions[dir]);

						if (rect.contains(point) && !map[point] && judge(image[point])) { map[point] = true; points.emplace_back(point); }
					}
				}
			}

			// 読み取り位置変更
			if (++p.x == rect.w)
			{
				p.x = 0;

				if (++p.y == rect.h) break;
			}
		}
	}

	// 結果を格納
	Array<Array<Vec2>> outlines;
	if (startPositions.empty()) return outlines;

	int		botDirection = 0;
	Point	botPosition = startPositions.back();
	startPositions.pop_back();

	Array<Point> outline;
	outline.emplace_back(botPosition);

	for (;;)
	{
		for (int i = -2; i <= 4; ++i)
		{
			// 始点に戻った場合
			if (outline.size() >= 2 && (botDirection + i) % 8 == 0 && botPosition == outline.front())
			{
				// 最初と最後を被らせないために末尾を削除する
				outline.pop_back();

				// 転写
				auto& result = outlines.emplace_back();
				for (const auto& o : outline)
					result.emplace_back(o.x, o.y);

				if (startPositions.empty()) return outlines;

				botPosition = startPositions.back();
				startPositions.pop_back();

				outline.clear();
				outline.emplace_back(botPosition);

				break;
			}

			// 移動
			const Point forwardPosition = botPosition + directions[(botDirection + i + 8) % 8];
			if (rect.contains(forwardPosition) && map[forwardPosition.y][forwardPosition.x])
			{
				botPosition = forwardPosition;
				botDirection = ((botDirection + i) % 8 + 8) % 8;
				outline.emplace_back(botPosition);

				break;
			}

			// 一ブロックの場合は無視する
			if (i == 4)
			{
				if (startPositions.empty()) return outlines;

				botPosition = startPositions.back();
				startPositions.pop_back();

				outline.clear();
				outline.emplace_back(botPosition);
			}
		}
	}
}

Array<Vec2> MainViewer::ImageDialog::getOutline(const Grid<bool>& map)
{
	const Point directions[8] = { { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 } };
	const Rect rect(map.size());
	Grid<bool> swap = map;
	Point startPosition;

	// mapに転写
	Point p = Point::Zero();
	for (;;)
	{
		if (swap[p])
		{
			Array<Point> points;

			// 始点
			startPosition = p;
			swap[p] = false;
			points.emplace_back(p);

			for (int i = 0; i < points.size(); ++i)
			{
				for (int dir = 0; dir < 8; ++dir)
				{
					const Point point = points[i].movedBy(directions[dir]);

					if (rect.contains(point) && swap[point]) { swap[point] = false; points.emplace_back(point); }
				}
			}

			break;
		}

		// 読み取り位置変更
		if (++p.x == rect.w)
		{
			p.x = 0;

			// 結果無し
			if (++p.y == rect.h) return Array<Vec2>();
		}
	}

	int		botDirection = 0;
	Point	botPosition = startPosition;

	Array<Point> outline;
	outline.emplace_back(botPosition);

	for (;;)
	{
		for (int i = -2; i <= 4; ++i)
		{
			// 始点に戻った場合
			if (outline.size() >= 2 && (botDirection + i) % 8 == 0 && botPosition == outline.front())
			{
				// 最初と最後を被らせないために末尾を削除する
				outline.pop_back();

				// 転写
				Array<Vec2> result;
				for (const auto& o : outline)
					result.emplace_back(o.x, o.y);

				return result;
			}

			// 移動
			const Point forwardPosition = botPosition + directions[(botDirection + i + 8) % 8];
			if (rect.contains(forwardPosition) && map[forwardPosition.y][forwardPosition.x])
			{
				botPosition = forwardPosition;
				botDirection = ((botDirection + i) % 8 + 8) % 8;
				outline.emplace_back(botPosition);

				break;
			}

			// 一ブロックの場合は無視
			if (i == 4) return Array<Vec2>();
		}
	}
}

Array<Array<Vec2>> MainViewer::ImageDialog::getOutlines(const Grid<bool>& map)
{
	const Point directions[8] = { { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 } };
	const Rect rect(map.size());
	Grid<bool> swap = map;
	Array<Point> startPositions;

	// mapに転写
	{
		Point p = Point::Zero();
		for (;;)
		{
			if (swap[p])
			{
				Array<Point> points;

				// 始点
				startPositions.emplace_back(p);
				swap[p] = false;
				points.emplace_back(p);

				for (int i = 0; i < points.size(); ++i)
				{
					for (int dir = 0; dir < 8; ++dir)
					{
						const Point point = points[i].movedBy(directions[dir]);

						if (rect.contains(point) && swap[point]) { swap[point] = false; points.emplace_back(point); }
					}
				}
			}

			// 読み取り位置変更
			if (++p.x == rect.w)
			{
				p.x = 0;

				if (++p.y == rect.h) break;
			}
		}
	}

	// 結果を格納
	Array<Array<Vec2>> outlines;
	if (startPositions.empty()) return outlines;

	int		botDirection = 0;
	Point	botPosition = startPositions.back();
	startPositions.pop_back();

	Array<Point> outline;
	outline.emplace_back(botPosition);

	for (;;)
	{
		for (int i = -2; i <= 4; ++i)
		{
			// 始点に戻った場合
			if (outline.size() >= 2 && (botDirection + i) % 8 == 0 && botPosition == outline.front())
			{
				// 最初と最後を被らせないために末尾を削除する
				outline.pop_back();

				// 転写
				auto& result = outlines.emplace_back();
				for (const auto& o : outline)
					result.emplace_back(o.x, o.y);

				if (startPositions.empty()) return outlines;

				botPosition = startPositions.back();
				startPositions.pop_back();

				outline.clear();
				outline.emplace_back(botPosition);

				break;
			}

			// 移動
			const Point forwardPosition = botPosition + directions[(botDirection + i + 8) % 8];
			if (rect.contains(forwardPosition) && map[forwardPosition.y][forwardPosition.x])
			{
				botPosition = forwardPosition;
				botDirection = ((botDirection + i) % 8 + 8) % 8;
				outline.emplace_back(botPosition);

				break;
			}

			// 一ブロックの場合は無視する
			if (i == 4)
			{
				if (startPositions.empty()) return outlines;

				botPosition = startPositions.back();
				startPositions.pop_back();

				outline.clear();
				outline.emplace_back(botPosition);
			}
		}
	}
}

MainViewer::ImageDialog::ImageDialog(const DroppedFilePath& droppedFilePath, const Image& image)
	: m_droppedFilePath(droppedFilePath)
	, m_image(image)
	, m_texture(image)
{
	m_multiPolygon = m_image.grayscaleToPolygons(160, false);
}

void MainViewer::ImageDialog::onLoad()
{
	const auto outers = getOutlines(m_image, [](const Color& color) { return color.grayscale() < 0.4; });

	for (const auto& outer : outers)
	{
		const Polygon polygon(outer);

		getParentViewer<MainViewer>()->getChildViewer<Workspace>()->addPolygon(polygon.scaled(25.4 / Parse<double>(getChildViewer<GUITextBox>()->m_textEditState.text)));
	}

	destroy();
}

void MainViewer::ImageDialog::init()
{
	setViewerSize(640, 480);

	addChildViewer<GUIButton>(U"読み込み", [this]() { onLoad(); })
		->setViewerRectInLocal(20, 420, 80, 30);

	addChildViewer<GUIButton>(U"閉じる", [this]() { destroy(); })
		->setViewerRectInLocal(120, 420, 80, 30);

	addChildViewer<GUITextBox>(U"350")
		->setViewerRectInLocal(320, 5, 90, 30);
}

void MainViewer::ImageDialog::update()
{
	setViewerPosInLocal(Scene::Center() - getViewerSize() / 2.0);

	RectF(getViewerSize()).draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);

	// preview (240, 240)
	{
		const RectF region = RectF(320, 320).stretched(-5).draw(ColorF(0.25));
		const double s = Min(region.w / m_image.width(), region.h / m_image.height());
		const auto t = Transformer2D(Mat3x2::Translate(-m_image.size() / 2).scaled(s).translated(region.center()));

		m_texture.draw(ColorF(0.5));
		m_multiPolygon.drawFrame(2.0 * s, Palette::Green);
	}

	// PPIの選択
	{
		const double ppi = Parse<double>(getChildViewer<GUITextBox>()->m_textEditState.text);
		const double w = m_image.width() / ppi * 25.4;
		const double h = m_image.height() / ppi * 25.4;
		if (ppi > 0)
		{
			static Font font(20);

			font(U"PPI:", ppi).draw(320, 40, Palette::Black);
			font(w, U"mm x ", h, U"mm").draw(320, 70, Palette::Black);
		}
	}
}
