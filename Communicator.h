#pragma once

#include "Command.h"

class Communicator
{
	Array<Command>	m_commands;
	Serial	m_serial;
	bool	m_canSend = true;

public:
	bool	connect(String portname) { return m_serial.open(portname, 115200); }

	void	addCommand(const Command& command) { m_commands.emplace_back(command); }
	template<typename... Args>
	bool	addCommand(Args&&... args) { m_commands.emplace_back(args); }

	void	update();
};
