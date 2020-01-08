#pragma once

#include "EasyViewer.h"

class GUIText :
	public EasyViewer
{
	String	m_text;

public:
	GUIText(const String& text)
		: m_text(text)
	{}

	void	update() override;
};
