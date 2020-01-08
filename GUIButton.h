﻿#pragma once

#include "EasyViewer.h"

class GUIButton :
	public EasyViewer
{
	bool	m_isGrabbed = false;
	bool	m_isSelected = false;
	bool	m_isEnabled = true;
	Color	m_color;
	String	m_text;
	std::function<void(void)>	m_functionOnSelected;

	Color	getTargetColor() const;

public:
	GUIButton(const String& text, bool isEnabled = true)
		: m_text(text)
		, m_isEnabled(isEnabled)
	{}
	GUIButton(const String& text, std::function<void(void)> functionOnSelected, bool isEnabled = true)
		: m_text(text) 
		, m_functionOnSelected(functionOnSelected)
		, m_isEnabled(isEnabled)
	{}

	void	init() override { m_color = getTargetColor(); }
	void	update() override;
	
	void	setIsEnabled(bool isEnabled);

	bool	isGrabbed() const { return m_isGrabbed; }
	bool	isEnabled() const { return m_isEnabled; }
};

