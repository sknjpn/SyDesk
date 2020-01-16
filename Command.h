#pragma once

#include "common.h"

class Command
{
public:
	char	m_code;
	short	m_parameters[4];

	Command(char code, short param0, short param1, short param2, short param3)
		: m_code(code)
	{
		m_parameters[0] = param0;
		m_parameters[1] = param1;
		m_parameters[2] = param2;
		m_parameters[3] = param3;
	}

	std::string asSendBuffer() const;
};

