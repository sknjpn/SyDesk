﻿#include "MainViewer.h"
#include "Node.h"

void MainViewer::Workspace::updateShapes()
{
	auto& routeGenerator = getParentViewer<MainViewer>()->m_routeGenerator;

	routeGenerator.m_cuttingMultiPolygons.clear();
	routeGenerator.m_circlingMultiPolygons.clear();

	for (auto& shape : m_shapes)
	{
		shape.update(routeGenerator);
		routeGenerator.m_circlingMultiPolygons.emplace_back(shape.m_circlingPolygon);
		routeGenerator.m_cuttingMultiPolygons.emplace_back(shape.m_cuttingPolygon);
	}

	routeGenerator.update();
}

void MainViewer::Workspace::addPolygon(const Polygon& polygon)
{
	m_shapes.emplace_back(polygon)
		.update(getParentViewer<MainViewer>()->m_routeGenerator);
}

void MainViewer::Workspace::onMarginChanged()
{
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

	RectF(routeGenerator.m_workspaceSize).drawFrame(2.0 / s, Palette::Darkkhaki);

	for (auto& shape : m_shapes)
	{
		shape.m_polygon.draw(Palette::Green);
		shape.m_circlingPolygon.boundingRect().drawFrame(1.0 / s, Palette::Blue);

		if (MouseL.up()) shape.m_isGrabbed = false;
		if (shape.m_polygon.leftClicked()) shape.m_isGrabbed = true;
		if (shape.m_isGrabbed)
		{
			shape.m_circlingPolygon.moveBy(Cursor::DeltaF());
			shape.m_cuttingPolygon.moveBy(Cursor::DeltaF());
			shape.m_polygon.moveBy(Cursor::DeltaF());

			m_needToUpdate = true;
		}
	}

	if (!m_inUpdate)
	{
		for (auto& shape : m_shapes)
		{
			shape.m_circlingPolygon.draw(ColorF(Palette::Blue, 0.25));

			const auto& outer = shape.m_circlingPolygon.outer();
			for (int i = 0; i < outer.size() - 1; ++i)
				Line(outer[i], outer[i + 1]).draw(2.0 / s, Palette::Blue);
		}

		for (auto& shape : m_shapes)
		{
			const auto& outer = shape.m_cuttingPolygon.outer();
			for (int i = 0; i < outer.size() - 1; ++i)
				Line(outer[i], outer[i + 1]).draw(2.0 / s, Palette::Red);
		}

		/*for (const auto& node1 : routeGenerator.m_nodes)
		{
			for (const auto& node2 : node1->m_connectedNodes)
				Line(node1->m_position, node2->m_position).draw(0.5 / s, Palette::Skyblue);

		}*/
		
		if(!routeGenerator.getRoute().isEmpty())
		for (int i = 0; i < routeGenerator.getRoute().size() - 1; ++i)
			Line(routeGenerator.getRoute()[i], routeGenerator.getRoute()[i + 1]).draw(2.0 / s, Palette::Red);

		if (KeyEnter.down()) getParentViewer<MainViewer>()->m_communicator.addCommands(getParentViewer<MainViewer>()->m_routeGenerator.getCommands());
	}
}
