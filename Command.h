#pragma once

#include "common.h"

class Command
{
	char	m_code;
	short	m_parameters[4];

public:
	
	std::string asSendBuffer() const;
};

