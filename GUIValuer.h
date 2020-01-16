#pragma once

#include "EasyViewer.h"

class GUIValuer
	: public EasyViewer
{
	bool	m_isGrabbed = false;
	double	m_grabbedPos;
	Color	m_color;
	std::function<void(double)>	m_functionOnSelected;

	Circle	getButton() const;

	Color	getTargetColor() const { return m_isGrabbed ? Color(192) : isMouseover() ? Palette::Skyblue : Palette::White; }

public:
	double	m_value;

public:
	GUIValuer(double value = 0.0)
		: m_value(value)
	{}
	GUIValuer(std::function<void(double)> functionOnSelected, double value = 0.0)
		: m_functionOnSelected(functionOnSelected)
		, m_value(value)
	{}

	void	init() override;
	void	update() override;

	bool	isGrabbed() const { return m_isGrabbed; }
};
