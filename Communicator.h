#pragma once

#include "Command.h"

class Communicator
{
	Array<Command>	m_commands;
	Serial	m_serial;
	bool	m_isConnected = false;

	bool	m_canSend = true;
	bool	m_canSend_sub = true;

	void	updateRecv();
	void	updateSend();

public:
	bool	connect(String portname);

	void	addCommands(const Array<Command>& commands) { m_commands.append(commands); }
	void	addCommandAtFront(const Command& command) { m_commands.push_front(command); }
	void	addCommand(const Command& command) { m_commands.emplace_back(command); }
	void	addCommand(char code, short param0, short param1, short param2, short param3) { m_commands.emplace_back(code, param0, param1, param2, param3); }

	const Serial& getSerial() const { return m_serial; }

	bool	isConnected() { return m_isConnected; }

	void	update();
};
