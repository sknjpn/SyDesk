#pragma once

#include "EasyViewer.h"

class GUITextBox :
	public EasyViewer
{
public:
	TextEditState	m_textEditState;
	std::function<void(const String&)>	m_functionOnChanged;

public:
	GUITextBox(const String& text = U"")
		: m_textEditState(text)
	{}
	GUITextBox(std::function<void(const String&)> functionOnChanged, const String& text = U"")
		: m_functionOnChanged(functionOnChanged)
		, m_textEditState(text)
	{}

	void	update() override;
};

