#pragma once

#include "Command.h"

class Communicator
{
	Array<Command>	m_commands;
	Serial	m_serial;

public:
	bool	connect(String portname) { return m_serial.open(portname, 115200); }
};
