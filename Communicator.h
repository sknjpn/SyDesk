#pragma once

#include "Command.h"

class Communicator
{
	Array<Command>	m_commands;
	Serial	m_serial;
	bool	m_canSend = true;

public:
	bool	connect(String portname) { return m_serial.open(portname, 115200); }

	void	addCommands(const Array<Command>& commands) { m_commands.append(commands); }
	void	addCommand(const Command& command) { m_commands.emplace_back(command); }
	void	addCommand(char code, short param0, short param1, short param2, short param3) { m_commands.emplace_back(code, param0, param1, param2, param3); }

	void	update();
};
