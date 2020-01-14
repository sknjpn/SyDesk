#pragma once

#include <Siv3D.hpp>

class Node
	: std::enable_shared_from_this<Node>
{
public:
	int m_shapeIndex = 0;
	int m_outerIndex = 0;
	Vec2 m_position = Vec2::Zero();

	double cost = 0;
	std::shared_ptr<Node> m_from = nullptr;

	Array<std::shared_ptr<Node>> m_connectedNodes;
};

