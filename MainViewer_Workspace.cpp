#include "MainViewer.h"
#include "Node.h"

void MainViewer::Workspace::updateShapes()
{
	auto& routeGenerator = getParentViewer<MainViewer>()->m_routeGenerator;

	// Shapes
	{
		Array<ConcurrentTask<void>> tasks;

		for (auto& shape : m_shapes)
			if (shape.isNeedUpdate())  tasks.emplace_back(CreateConcurrentTask([&shape, &routeGenerator]() { shape.update(routeGenerator); }));

		for (const auto& task : tasks)
			while (!task.is_done()) std::this_thread::sleep_for(std::chrono::microseconds(1));

		// routeGeneratorにデータ移動
		{
			MultiPolygon cuttingPolygons;
			MultiPolygon circlingPolygons;

			for (auto& shape : m_shapes)
			{
				cuttingPolygons.emplace_back(shape.getCuttingPolygon());
				circlingPolygons.emplace_back(shape.getCirclingPolygon());
			}

			// コピー
			{
				routeGenerator.setCuttingPolygons(cuttingPolygons);
				routeGenerator.setCirclingPolygons(circlingPolygons);
			}
		}
	}

	routeGenerator.update();
}

void MainViewer::Workspace::addPolygon(const Polygon& polygon)
{
	m_shapes.emplace_back(polygon);

	m_needToUpdate = true;
}

void MainViewer::Workspace::onMarginChanged()
{
	for (auto& shape : m_shapes)
		shape.setNeedUpdate();

	m_needToUpdate = true;
}

void MainViewer::Workspace::init()
{
	m_updater = CreateConcurrentTask(&MainViewer::Workspace::update, this);
	m_inUpdate = true;
}

void MainViewer::Workspace::update()
{
	const auto& routeGenerator = getParentViewer<MainViewer>()->m_routeGenerator;

	RectF(getViewerSize()).stretched(-5).draw(ColorF(0.9)).drawFrame(2.0, 0.0, Palette::Black);

	auto s = 4.0;
	auto t = Transformer2D(Mat3x2::Scale(s), true);

	if (m_inUpdate && m_updater.is_done()) m_inUpdate = false;

	if (m_needToUpdate && !m_inUpdate)
	{
		m_needToUpdate = false;
		m_inUpdate = true;

		m_updater = CreateConcurrentTask(&MainViewer::Workspace::updateShapes, this);
	}

	RectF(routeGenerator.getWorkspaceSize()).drawFrame(2.0 / s, Palette::Darkkhaki);

	for (int i = m_shapes.size() - 1; i >= 0; --i)
		if (m_shapes[i].getPolygon().boundingRect().stretched(routeGenerator.getCirclingMargin() + routeGenerator.getCuttingMargin()).leftClicked()) { m_shapes[i].m_isGrabbed = true; break; }

	for (auto it = m_shapes.begin(); it != m_shapes.end(); ++it)
		if (it->getPolygon().boundingRect().stretched(routeGenerator.getCirclingMargin() + routeGenerator.getCuttingMargin()).rightClicked()) { m_shapes.erase(it); m_needToUpdate = true; return; }

	for (auto& shape : m_shapes)
	{
		if (MouseL.up()) shape.m_isGrabbed = false;
		if (shape.m_isGrabbed)
		{
			m_needToUpdate = true;

			shape.MoveBy(Cursor::DeltaF());
		}
	}

	for (auto& shape : m_shapes)
		shape.draw(routeGenerator, s);

	if (!m_inUpdate)
	{
		if (!routeGenerator.getRoute().isEmpty())
			for (const auto& p : routeGenerator.getRoute())
				Circle(p, 1.0 / s).draw(Palette::Red);
			//for (int i = 0; i < routeGenerator.getRoute().size() - 1; ++i)
				//Line(routeGenerator.getRoute()[i], routeGenerator.getRoute()[i + 1]).draw(2.0 / s, Palette::Red);
	}
}
